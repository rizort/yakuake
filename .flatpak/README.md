# Local Flatpak build

This manifest builds the checked-out Yakuake tree as `org.kde.yakuake.Devel`.
It can be installed alongside Flathub's `org.kde.yakuake`.

```bash
flatpak-builder --force-clean --user --install-deps-from=flathub \\
  --repo=flatpak-repo --install build-flatpak \\
  .flatpak/org.kde.yakuake.Devel.json

flatpak run org.kde.yakuake.Devel
```

The first build downloads the KDE runtime and builds Konsole. Later builds
reuse the `flatpak-repo/` and `.flatpak-builder/` caches. To start over, remove
only `build-flatpak/`, `flatpak-repo/`, and `.flatpak-builder/`.

The manifest also renames Yakuake's D-Bus activation service and its internal
D-Bus name for the development application; this prevents a conflict with the
stable Yakuake instance.
