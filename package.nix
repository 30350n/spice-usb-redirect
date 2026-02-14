{
    lib,
    gtk3,
    meson,
    ninja,
    pkg-config,
    spice-gtk,
    spice-protocol,
    stdenv,
    wrapGAppsHook3,
}:
stdenv.mkDerivation {
    pname = "spice-usb-redirect";
    version = "0.1.0";

    src = ./.;

    nativeBuildInputs = [
        meson
        ninja
        pkg-config
        wrapGAppsHook3
    ];

    buildInputs = [
        gtk3
        spice-gtk
        spice-protocol
    ];

    propagatedUserEnvPkgs = [spice-gtk];

    meta = {
        description = "Minimal Spice USB redirection GTK app";
        homepage = "https://github.com/30350n/spice-usb-redirect";
        license = lib.licenses.gpl3;
    };
}
