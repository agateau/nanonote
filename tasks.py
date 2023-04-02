import os
import re
import shutil
import sys

from pathlib import Path
from tempfile import NamedTemporaryFile
from typing import List

from invoke import task, run

from changelog import Changelog, Release


ARTIFACTS_DIR = Path("artifacts")

CHANGELOG_MD = Path("CHANGELOG.md")

MAIN_BRANCH = "master"


def get_version():
    return os.environ["VERSION"]


def erun(*args, **kwargs):
    """Like run, but with echo on"""
    kwargs["echo"] = True
    return run(*args, **kwargs)


def cerun(c, *args, **kwargs):
    """Like Context.run, but with echo on"""
    kwargs["echo"] = True
    return c.run(*args, **kwargs)


def ask(msg: str) -> str:
    """Show a message, wait for input and returns it"""
    print(msg, end=" ")
    return input()


def is_ok(msg: str) -> bool:
    """Show a message, append (y/n) and return True if user select y or Y"""
    answer = ask(f"{msg} (y/n)").lower()
    return answer == "y"


@task
def create_pr(c):
    """Create a pull-request and mark it as auto-mergeable"""
    result = cerun(c, "gh pr create --fill", warn=True)
    if not result:
        sys.exit(1)
    cerun(c, "gh pr merge --auto -dm")


@task
def update_version(c):
    version = get_version()
    path = Path("CMakeLists.txt")
    text = path.read_text()
    text, count = re.subn(
        r"^    VERSION .*", f'    VERSION {version}', text, flags=re.MULTILINE
    )
    assert count == 0 or count == 1
    path.write_text(text)


@task
def create_release_branch(c):
    version = get_version()
    run(f"gh issue list -m {version}", pty=True)
    run("gh pr list", pty=True)
    if not is_ok("Continue?"):
        sys.exit(1)

    erun(f"git checkout {MAIN_BRANCH}")
    erun("git pull")
    erun("git status -s")
    if not is_ok("Continue?"):
        sys.exit(1)

    create_release_branch2(c)


@task
def create_release_branch2(c):
    version = get_version()
    erun("git checkout -b prep-release")

    update_version(c)

    erun(f"changie batch {version}")
    print(f"Review/edit changelog (.changes/{version}.md)")
    if not is_ok("Looks good?"):
        sys.exit(1)
    erun("changie merge")
    print("Review CHANGELOG.md")

    if not is_ok("Looks good?"):
        sys.exit(1)


@task
def update_ts(c):
    erun("ninja -C build lupdate")
    erun("git add src/translations")
    erun("git commit -m 'Update translations'")


@task
def commit_push(c):
    version = get_version()
    erun("git add .")
    erun(f'git commit -m "Prepare release of {version}"')
    erun("git push -u origin prepare-release")
    create_pr(c)


@task
def tag(c):
    version = get_version()
    erun(f"git checkout {MAIN_BRANCH}")
    erun("git pull")
    changes_file = Path(".changes") / f"{version}.md"
    if not changes_file.exists():
        print(f"{changes_file} does not exist, check previous PR has been merged")
        sys.exit(1)
    if not is_ok("Create tag?"):
        sys.exit(1)

    erun(f"git tag -a {version} -m 'Releasing version {version}'")

    erun(f"git push origin {version}")


def get_artifact_list() -> List[Path]:
    assert ARTIFACTS_DIR.exists()
    return list(ARTIFACTS_DIR.glob("*.rpm")) + list(ARTIFACTS_DIR.glob("*.deb"))


@task
def download_artifacts(c):
    if ARTIFACTS_DIR.exists():
        shutil.rmtree(ARTIFACTS_DIR)
    ARTIFACTS_DIR.mkdir()
    erun(f"gh run download --dir {ARTIFACTS_DIR}", pty=True)


def prepare_release_notes(release: Release) -> str:
    """
    Take a Release instance and produce markdown suitable for GitHub release
    notes
    """
    lines = []
    for change_type, changes in release.changes.items():
        lines.append(f"## {change_type}")
        for change in changes:
            lines.append(f"- {change}")
    return "\n\n".join(lines) + "\n"


@task(help={"pre": "This is a prerelease"})
def publish(c, pre=False):
    version = get_version()
    changelog = Changelog.from_path(CHANGELOG_MD)
    release = changelog.releases[version]
    content = prepare_release_notes(release)

    files_str = " ".join(str(x) for x in get_artifact_list())
    with NamedTemporaryFile() as tmp_file:
        tmp_file.write(content.encode("utf-8"))
        tmp_file.flush()
        cmd = f"gh release create {version} -F{tmp_file.name} {files_str}"
        if pre:
            cmd += " --prerelease"
        erun(cmd)
