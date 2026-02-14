project(
    'spice-usb-redirect',
    'c',
    version: '0.1.0',
    default_options: [
        'c_std=gnu99',
        'warning_level=1',
    ],
)

gtk = dependency('gtk+-3.0')
spice_client_gtk = dependency('spice-client-gtk-3.0')

executable(
    'spice-usb-redirect',
    'spice-usb-redirect.c',
    dependencies: [gtk, spice_client_gtk],
    install: true,
)
