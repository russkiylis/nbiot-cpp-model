# Bundled dependencies

## GoogleTest

- Upstream: https://github.com/google/googletest
- Release: [v1.17.0](https://github.com/google/googletest/releases/tag/v1.17.0)
- Commit: `52eb8108c5bdec04579160ae17225d66034bd723`
- License: [BSD 3-Clause](googletest/LICENSE)

`googletest/` contains the unmodified source tree of the pinned upstream commit,
without Git metadata or build artifacts. Keep the upstream `LICENSE` when updating it.

With `ENABLE_GTEST=ON`, `src/CMakeLists.txt` builds these sources using
`add_subdirectory`; CMake does not download GoogleTest. GoogleTest 1.17.0
requires CMake 3.16 or later and C++17 or later.
