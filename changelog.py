from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Iterable


@dataclass
class Release:
    version: str
    date: str
    # type => [changes]
    changes: dict[str, list[str]] = field(default_factory=dict)


@dataclass
class Changelog:
    # version => release
    releases: dict[str, Release] = field(default_factory=dict)

    @staticmethod
    def from_path(changelog_path: Path) -> "Changelog":
        with changelog_path.open() as f:
            parser = Parser(f)
            return parser.parse()


def _get_dict_last_added_item(dct: dict[Any, Any]) -> Any:
    return list(dct.values())[-1]


class Parser:
    def __init__(self, line_it: Iterable[str]):
        self.changelog = Changelog()
        self.line_it = line_it

    def parse(self) -> Changelog:
        self._parser = self._parse_prolog
        for line in self.line_it:
            line = line.strip()
            if line:
                self._parser(line)

        return self.changelog

    def _parse_prolog(self, line: str) -> None:
        if line.startswith("## "):
            self._parse_release_title(line)
            self._parser = self._parse_release_content

    def _parse_release_title(self, line: str) -> None:
        version, date = line[3:].split(" - ", maxsplit=1)
        release = Release(version=version, date=date)
        self.changelog.releases[version] = release

    def _parse_release_content(self, line: str) -> None:
        if line.startswith("## "):
            self._parse_release_title(line)
            return

        release = _get_dict_last_added_item(self.changelog.releases)

        if line.startswith("### "):
            change_type = line[4:]
            release.changes[change_type] = []
        else:
            assert line.startswith("- "), line
            current_changes = _get_dict_last_added_item(release.changes)
            current_changes.append(line[2:])


if __name__ == "__main__":
    changelog = Changelog.from_path(Path("CHANGELOG.md"))
    print(changelog)
