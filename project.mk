PROJECT=Obbligato
PROJECT_NAME=Obbligato
PROJECT_VERSION?=20130106
PROJECT_EMAIL=jeffk@jdkoftinoff.com
PROJECT_LICENSE=ISC
PROJECT_MAINTAINER=jeffk@jdkoftinoff.com
PROJECT_COPYRIGHT=Copyright 2013
PROJECT_DESCRIPTION=
PROJECT_WEBSITE=https://obbligato.statusbar.com/
PROJECT_IDENTIFIER=org.statusbar.obbligato
TOP_LIB_DIRS+=.
CONFIG_TOOLS+=
PKGCONFIG_PACKAGES+=
TEST_OUT_SUFFIX=txt

CONFIG_TOOLS=

COMPILE_FLAGS+=-Wall -W
COMPILE_FLAGS+=-std=c++11

ifeq ($(TARGET_PLATFORM_MACOSX),1)
COMPILE_FLAGS+=-stdlib=libc++
LINK_FLAGS+=-stdlib=libc++
COMPILER=clang
MAC_OSX_VERSION?=10.8
MAC_SDK?=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$(MAC_OSX_VERSION).sdk
COMPILE_FLAGS+=-mmacosx-version-min=$(MAC_OSX_VERSION)
LINK_FLAGS+=-mmacosx-version-min=$(MAC_OSX_VERSION) -Wl,-syslibroot,$(MAC_SDK)
endif


