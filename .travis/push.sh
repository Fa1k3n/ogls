#!/bin/sh

setup_git() {
  git config --global user.email "johan.falkenstrom@gmail.com"
  git config --global user.name "Travis CI"
}

commit_website_files() {
  git add docs
  git commit --message "Travis build: $TRAVIS_BUILD_NUMBER"
}

upload_files() {
  git remote add origin-pages https://${GITHUB_TOKEN}@github.com/fa1k3n/ogls.git > /dev/null 2>&1
  git push origin-pages master 
}

setup_git
commit_website_files
upload_files