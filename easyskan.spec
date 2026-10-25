Name:           easyskan
Version:        1.0.0
Release:        1%{?dist}
Summary:        Ergonomicky skenovaci CAD nastroj s RGB mixerem
Group:          Applications/Multimedia
License:        GPLv3
URL:            https://github.com
Source0:        %{name}-%{version}.tar.gz

# Hardwarove a softwarove zavislosti pro sestaveni a beh ve Fedore
BuildRequires:  make
BuildRequires:  gcc-c++
BuildRequires:  cmake
BuildRequires:  qt6-qtbase-devel
BuildRequires:  qt6-qttools-devel
BuildRequires:  sane-backends-devel

Requires:       qt6-qtbase
Requires:       sane-backends
Requires:       hicolor-icon-theme

%description
Ergonomicky skenovaci a orezovy CAD nastroj s RGB mixerem a poctivym ShowJPEG jadrem lupy.
Vyvinuto jako lehky a bleskurychly nastroj pro maximalni tiskovou a studiovou presnost 1:1.

%prep
# FIX: Přepínačem -n natvrdo donutíme rpmbuild, aby po rozbalení skočil do složky EasySkan!
%setup -q -n EasySkan

%build

# Poctive cmake sestaveni primo uvnitr baliciho procesu RedHatu
%cmake
%cmake_build

%install
# Nainstalujeme binarku do docasneho rootu balicku
%cmake_install

# Rucne ulozime systemovy spustec a nasi luxusni 3D prostorovou ikonu
mkdir -p %{buildroot}%{_datadir}/applications
mkdir -p %{buildroot}%{_datadir}/icons/hicolor/256x256/apps

cp easyskan.desktop %{buildroot}%{_datadir}/applications/
cp easyskan.png %{buildroot}%{_datadir}/icons/hicolor/256x256/apps/

%files
%{_bindir}/easyskan
%{_datadir}/applications/easyskan.desktop
%{_datadir}/icons/hicolor/256x256/apps/easyskan.png

%changelog
* Sun Oct 25 2026 Jirka <jirka@jirkastudio.cz> - 1.0.0-1
- Prechod na plnohodnotne archivni RPM baleni (.tar.gz + spec)
- Pridany striktni systemove zavislosti pro Qt6 a SANE
- Plna integrace hicolor ikonoveho tematu Fedory
