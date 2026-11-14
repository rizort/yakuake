APP_ID := org.kde.yakuake.Devel
MANIFEST := .flatpak/$(APP_ID).json
BUILD_DIR := build-flatpak
REPO_DIR := flatpak-repo
BUNDLE := yakuake-devel.flatpak

.PHONY: help build flatpak-build flatpak-run flatpak-bundle flatpak-uninstall flatpak-purge

help:
	@echo "make build             Build Yakuake natively in build/"
	@echo "make flatpak-build     Build and install $(APP_ID)"
	@echo "make flatpak-run       Run $(APP_ID)"
	@echo "make flatpak-bundle    Create $(BUNDLE)"
	@echo "make flatpak-uninstall Remove $(APP_ID), keeping its settings"
	@echo "make flatpak-purge     Remove $(APP_ID) and all its settings"

build:
	cmake --build build

flatpak-build:
	flatpak-builder --force-clean --user --install-deps-from=flathub --repo=$(REPO_DIR) --install $(BUILD_DIR) $(MANIFEST)

flatpak-run:
	flatpak run $(APP_ID)

flatpak-bundle:
	flatpak build-bundle $(REPO_DIR) $(BUNDLE) $(APP_ID) --runtime-repo=https://dl.flathub.org/repo/flathub.flatpakrepo

flatpak-uninstall:
	flatpak uninstall --user $(APP_ID)

flatpak-purge:
	flatpak uninstall --user --delete-data $(APP_ID)
