#!/usr/bin/bash

cd dependencies/VL

statusResult=$(git status | grep "Changes not staged for commit")
if [ ! -z "${statusResult}" ]
then
	echo "Stash local work"
	git stash -m "pull_vl.sh script stash"
fi

echo "Pull"
branch=$(git branch --show-current)
git pull origin ${branch} --rebase

if [ ! -z "${statusResult}" ]
then
	echo "Pop back the local work"
	stashListResult=$(git stash list | grep "pull_vl.sh script stash")
	if [ ! -z "${stashListResult}" ]
	then
		git stash pop
	fi
fi
cd ../..


