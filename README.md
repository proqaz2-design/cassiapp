# Cassia (fork)

This fork makes it easier to build when the external `cassiaext` project is not available locally.

Quick notes:
- To build using a prebuilt `cassiaext` tarball, set `cassiaext.downloadUrl` in `gradle.properties` to a URL returning a tar.gz containing the prefix (the build task will download it and include it as assets).
- Alternatively, create `app/cassiaext.cfg` pointing to a local CassiaExt build directory as before.
- To produce a release-signed APK, set the following properties in `gradle.properties`: `keystore.storeFile`, `keystore.storePassword`, `keystore.keyAlias`, `keystore.keyPassword`.

CI:
- A minimal GitHub Actions workflow is provided at `.github/workflows/android-ci.yml` which runs `./gradlew assembleDebug`.
