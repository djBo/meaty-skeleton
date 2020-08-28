#!/bin/bash

build() {
	make -C "$1" "$2"
}

download() {
	if [ -z "$2" ]; then
		git clone "$1"
	else
		git clone --branch "$2" "$1"
	fi
}

main() {
	if [ $# -lt 2 ]; then
		echo "Usage: $0 <git-repo> <make-target> [branch]"
		exit 1
	fi
	download "$1" "$3"
	build $(basename "$1" ".${1##*.}") "$2"
}

main "$@"
