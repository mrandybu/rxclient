# Enable USBIP support
%def_without usbip

%define oname opennx

Name: rxclient
Version: 0.17
Release: alt1

Summary: A client for RX@Etersoft Terminal Server

License: LGPL/GPL
Group: Networking/Remote access
Url: http://sourceforge.net/projects/opennx

Packager: Vitaly Lipatov <lav@altlinux.ru>

Source: %oname-%version.tar

%if_with usbip
Requires: usbip2-nxclient
%endif

# Automatically added by buildreq on Sat Sep 19 2009
BuildRequires: gcc-c++ imake libSM-devel libXmu-devel
BuildRequires: libopensc-devel libsmbclient-devel
BuildRequires: xorg-cf-files zip libcups-devel
BuildRequires: libXau-devel
BuildRequires: libwxGTK3.1-devel
# check:
BuildRequires: nx

#Requires: nxssh

# dynamic loading
Requires: libcups libsmbclient

%description
RX Client is a NX 3.5 compatible client based on OpenNX code.

%prep
%setup

%build
%autoreconf
%configure \
    --localedir=%_datadir/locale \
%if_with usbip
    --enable-usbip \
%endif
    --with-nxproto=3.3.0

%make_build

%install
%makeinstall_std
# socks support
#rm -f %buildroot%_bindir/pconnect %buildroot%_datadir/pconnect.html

rm -f %buildroot%_desktopdir/*.directory

%if_with usbip
install -d -m 755 %buildroot%_sysconfdir/udev/rules.d
install -m 644 etc/*.rules %buildroot%_sysconfdir/udev/rules.d
%endif

%find_lang %name

%if_with usbip
%pre
%_sbindir/groupadd -r opennx || :
%endif

%files -f %name.lang
%_bindir/%name
%_bindir/pconnect
%_bindir/watchreader
%_datadir/%name
%_desktopdir/*.desktop
%_liconsdir/*.png
%_niconsdir/*.png
%_miconsdir/*.png
%_iconsdir/hicolor/128x128/apps/*.png
%_iconsdir/hicolor/256x256/apps/*.png
%_iconsdir/hicolor/512x512/apps/*.png
%_iconsdir/hicolor/scalable/apps/*.svg
%_iconsdir/hicolor/*/mimetypes/rx-desktop.*
%if_with usbip
%_sysconfdir/udev
%endif

%changelog
* Fri Dec 23 2016 Vitaly Lipatov <lav@altlinux.ru> 0.17-alt1
- initial build RX Client 0.17 for RX@Etersoft Terminal Server 1.1.4
- update russian translation
- add nxssh module support

* Fri May 20 2016 Vitaly Lipatov <lav@altlinux.ru> 0.16.e-alt32
- fix big using d instead ld at config session window
- append to commit "Now all paths write to config after start." ( 47816fbff3e6f4aee4142ae843fa3edda529ac77 )
- merge branch 'mainline' into sisyphus

* Fri Apr 22 2016 Vitaly Lipatov <lav@altlinux.ru> 0.16.e-alt31
- fix using string in login dialogs combobox

* Mon Oct 05 2015 Vitaly Lipatov <lav@altlinux.ru> 0.16.e-alt30
- fixed exit message: exit function now accept exit message
- fixed 'no sessions' bug
- fixed possible mistakes in code, added version dependency

* Sun Oct 04 2015 Anton Midyukov <antohami@altlinux.org> 0.16.e-alt29.svn724
- Rebuilt for new gcc5 C++11 ABI.

* Wed Aug 12 2015 Vitaly Lipatov <lav@altlinux.ru> 0.16.e-alt28.svn724
- fixed wrong cast, fixed 3.1.0 building

* Tue Aug 11 2015 Vitaly Lipatov <lav@altlinux.ru> 0.16.e-alt27.svn724
- fix release for build in ALT Linux

* Tue Aug 11 2015 Vitaly Lipatov <lav@altlinux.ru> 0.16-eter27.svn724
- add .gear in sisyphus branch
- fix compiles in all versions of wxWidgets (2.8, 2.9, 3.0, 3.1)
- do autoreconf

* Sun Jan 26 2014 Vitaly Lipatov <lav@altlinux.ru> 0.16-eter26.svn724
- fix build with libsmbclient from samba 4.0, fix symlink to libsmbclient

* Thu Oct 03 2013 Vitaly Lipatov <lav@altlinux.ru> 0.16-eter25.svn724
- remove PidFile option from cups config (eterbug #9490)

* Mon Aug 12 2013 Vitaly Lipatov <lav@altlinux.ru> 0.16-eter24.svn724
- use absolute path for links, fix requires

* Mon Aug 05 2013 Vitaly Lipatov <lav@altlinux.ru> 0.16-eter23.svn724
- add Num Lock state as a parameter to be send

* Mon Jun 10 2013 Denis Baranov <baraka@altlinux.ru> 0.16-eter22.svn724
- Add patch for translate

* Mon Feb 11 2013 Denis Baranov <baraka@altlinux.ru> 0.16-eter21.svn724
- Now all paths write to config after start

* Fri Jan 18 2013 Denis Baranov <baraka@altlinux.ru> 0.16-eter20.svn724
- Translate of Resume button correct
- Update translations
- Add translate for Wizard button

* Thu Jan 17 2013 Denis Baranov <baraka@altlinux.ru> 0.16-eter19.svn724
- Add copy icon in spec

* Fri Jan 04 2013 Denis Baranov <baraka@altlinux.ru> 0.16-eter18.svn724
- Add wizard button to start menu

* Thu Jan 03 2013 Denis Baranov <baraka@altlinux.ru> 0.16-eter17.svn724
- update from trunk opennx-0.16.0.724

* Fri Jun 01 2012 Denis Baranov <baraka@altlinux.ru> 0.16-eter17.svn708
- Initial build
- update from ALTLinux

* Fri Jun 17 2011 Boris Savelev <boris@altlinux.org> 0.16-alt16.svn634
- update from trunk

* Wed Mar 02 2011 Boris Savelev <boris@altlinux.org> 0.16-alt15.svn611
- update from trunk

* Mon Feb 28 2011 Lenar Shakirov <snejok@altlinux.ru> 0.16-alt14.svn595
- build fixed: disable opensc support by default

* Tue Feb 15 2011 Lenar Shakirov <snejok@altlinux.ru> 0.16-alt13.svn595
- lib{smbclient,cups}.so symlinks packaging fixed for x86_64

* Thu Nov 18 2010 Boris Savelev <boris@altlinux.org> 0.16-alt12.svn595
- update from trunk

* Sat Nov 13 2010 Lenar Shakirov <snejok@altlinux.ru> 0.16-alt11.svn567
- package nx-desktop.png (closes: #24467)

* Thu Sep 09 2010 Boris Savelev <boris@altlinux.org> 0.16-alt10.svn567
- update from trunk

* Tue Aug 31 2010 Boris Savelev <boris@altlinux.org> 0.16-alt9.svn555
- update from trunk

* Mon Aug 02 2010 Boris Savelev <boris@altlinux.org> 0.16-alt8.svn547
- update from trunk
- fix build

* Sat Feb 13 2010 Boris Savelev <boris@altlinux.org> 0.16-alt7.svn481
- update from trunk

* Mon Jan 25 2010 Boris Savelev <boris@altlinux.org> 0.16-alt6.svn450
- update from trunk (work with proxy with authorization)

* Sat Jan 16 2010 Boris Savelev <boris@altlinux.org> 0.16-alt5.svn446
- update from trunk
- add russian localization

* Thu Nov 12 2009 Boris Savelev <boris@altlinux.org> 0.16-alt4.svn444
- update from trunk
- add symlink for nxproxy

* Sun Oct 11 2009 Boris Savelev <boris@altlinux.org> 0.16-alt3.svn442
- update from trunk
- add symlinks for cups and samba

* Thu Sep 24 2009 Boris Savelev <boris@altlinux.org> 0.16-alt2.svn427
- update buildreq
- fix repocop warning

* Sat Sep 19 2009 Boris Savelev <boris@altlinux.org> 0.16-alt1.svn418
- intial build for Sisyphus

* Sun Apr 19 2009 Fritz Elfert <fritz@fritz-elfert.de>
- Set prefix to /opt/lsb/%name for FHS compliance
* Wed Apr 15 2009 Michael Kromer <michael.kromer@millenux.com>
- Fixes for SuSE Plattform (openSuSE/SLES)
* Sun Jan  7 2007 Fritz Elfert <fritz@fritz-elfert.de>
- Initial package