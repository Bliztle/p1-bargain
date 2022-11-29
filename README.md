![example event parameter]https://github.com/Bliztle/p1-bargain/blob/main/.github/workflows/cmake-tests.yml/badge.svg?event=push

# p1-bargain
This is a description of our P1 project.

## Git
This repo contains sub-repositories for most dependencies, and as such, should be cloned using the recursive flag
> `git clone https://github.com/Bliztle/p1-bargain --recursive`

The repo is set up such that no one but the owner has permissions to push directly to the main branch. Instead, everyone must push their changes to seperate branches, and then create a pull request for someone else to sign off on.

## Setup
This project comes with setup for cmake, but does **NOT** include any build tools. Depending on your OS you may face various issues setting up / building the project, so we recommend using either github codespaces or a linux distro set up for development. Alternatively, this repo includes a few DLL's which may be needed for windows development, located in `cmake-build-debug/src` - the default build directory when using CLion.

## Dependencies
- Included
    - libcurl v7.86.0
    - nxJson vCurrent
- Not included
    - openssl
    - libssl-dev

Openssl isn't included by default, due to being too OS specific. It should come pre-installed on windows, and can easily be installed on linux. MacOS users should look into installing `homebrew`.

### Linux
General linux install options. Look up the relevant variant for your distro

> `sudo apt install openssl -y`

> `sudo apt install libssl-dev -y`