# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## 0.2.0 - 2021-01-25
### Changed
- sequence size is now retrievable through size() static function instead of size enum
- map, filter and reduce now also pass the index of the element being processed
- improved the implementation of some of the algorithms to allow usage of longer sequences

## 0.1.1 - 2018-08-04
### Added
- .clang-format file
- .travis.yml build configuration file
- .appveyor.yml build configuration file

## 0.1.0 - 2018-08-04
### Added
- Initial development release of the library
- Test module that covers all current facilities provided by the library
- CMake modules that allow the installation and inclusion of the library into other projects
- README file that shows the basic API usage
- CHANGELOG file
- LICENSE file
