Check working tree is up to date and clean:

    git checkout dev
    git pull
    git status

Update CHANGELOG.md:

    r!git log --pretty=format:'- \%s (\%an)' x.y.z-1..HEAD

Bump version number in CMakeLists.txt

Commit and push

Build packages:

    docker run -v $PWD:/root/nanonote nanonote:1 /root/nanonote/ci/build-packages

Smoke test binary packages

- Test welcome text is OK
- Test screenshot matches

Merge dev in master:

    git checkout master
    git pull
    git merge --no-ff origin/dev

Create "x.y.z" tag:

    git tag -a x.y.z

Push:

    git push
    git push --tags

Publish packages on GitHub

Write blog post
