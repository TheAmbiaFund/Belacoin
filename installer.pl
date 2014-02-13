#!/usr/bin/perl -w

## BellaCoin Linux installer
## Revision 1
## E-mail: installer@bellacoin.com

## This installer must be ran as root for package management.  BellaCoin does not have to be run as root however.
if ($< != 0) {
  die("Sorry, for package management purposes this installer must be ran as root.\nBellaCoin does not have to be ran as root however once installation completes.\n");
}

print "\n
#### *DISCLAIMER* ####
# This script will make its best determinations to install supporting libraries and packages 
# for purposes of compiling and running the BellaCoin wallet (Qt & console).
# While there is a slim chance this will do any harm to your system, we must warn that we are
# not responsible for any damages claimed to have been caused by our script or coin.
# If you have any questions, please contact installer\@bellacoin.com.
####\n\n";

print "Do you wish to continue? (Type Y or N) ";
chomp($continue = <STDIN>);

if ($continue =~ /y/i) {

  ## Flavors this installer has been tested to work on
  %supported = ("Debian", "/etc/debian_version");

  foreach $distro (keys %supported) {
    if (-e $supported{$distro}) {
      $flavor = $distro;
    }
  }

  ## If non-supported, exit.  But don't worry, if it hasn't been tested, I WILL get to testing it and including support for it!
  if (!defined $flavor) {
    die("Sorry, your flavor of Linux hasn't been tested yet with this installer.\nIf you would like it to be added, please contact installer\@bellacoin.com\n");
  } else {
    &install_for($flavor);
  }

  sub install_for() {
    my $finstall = $_[0];
    my @errors;
    my $errnum = @errors;
    print "We will be installing on a $finstall system\n";
    ## Necessary programs
    @required_programs = ("gcc","automake","make","g++","qmake");
    foreach $program (@required_programs) {
      if (!-x "/usr/bin/$program") {
        print "Installing package for $program...\n";
	system("/usr/bin/apt-get -y install $program");
	if ($? != 0) {
	  push (@errors, $sys_lib);
        }
      }
    }
    %qt_programs = ("rcc","libqt4-dev-bin","qmlplugindump","qt4-dev-tools","lrelease","qt4-linguist-tools");
    foreach $qt_bin (keys %qt_programs) {
      if (!-x $qt_bin) {
        print "Installing $qt_programs{$qt_bin}...\n";
	system("/usr/bin/apt-get -y install $qt_programs{$qt_bin}");
	if ($? != 0) {
	  push (@errors, $sys_lib);
        }
      }
    }
    @sys_libs = ("libboost-dev","libssl-dev","libdb++-dev","libminiupnpc-dev","libboost-system-dev","libboost-filesystem-dev","libboost-program-options-dev","libboost-thread-dev");
    foreach $sys_lib (@sys_libs) {
      my $lib_result = `/usr/bin/dpkg -s $sys_lib >/dev/null 2>&1`;
      if(${^CHILD_ERROR_NATIVE} != 0) {
        print "Installing $sys_lib...\n";
	system("/usr/bin/apt-get -y install $sys_lib");
	if ($? != 0) {
	  push (@errors, $sys_lib);
        }
      } else {
 	print "$sys_lib is already installed, skipping.\n";
      }
    }

  if ($errnum == 0) {
    print "Installation of required packages and programs has succeeded.  Continuing onto compilation of BellaCoin\n";
    chomp($processors = `/bin/grep -c processor /proc/cpuinfo`);
    print "Compiling the Qt BellaCoin wallet...";
    system("/usr/bin/qmake");
    if ($? != 0) {
      print "Issue with qmake.  Please e-mail any output to installer\@bellacoin.com.\n";
    } else {
      system("make -j$processors");
      if ($? != 0) {
        print "Issue compiling the Qt wallet.  Please e-mail any output to installer\@bellacoin.com.\n";
      } else {
        system("cp BellaCoin-qt /usr/local/bin/");
	print "BellaCoin-qt has been installed in /usr/local/bin/\n";
      }
    }
    print "Compiling the console BellaCoin wallet...";
    print "\n\n\ncd src && /usr/bin/make -j$processors -f makefile.unix\n\n";
    system("cd src && /usr/bin/make -j$processors -f makefile.unix");
    if ($? != 0) {
      print "Issue compiling the console wallet.  Please e-mail any output to installer\@bellacoin.com.\n";
    } else {
      system("cd src && make -f makefile.unix install");
      if ($? != 0) {
        print "Issue installing the console wallet.  Please e-mail any output to installer\@bellacoin.com.\n";
      } else {
        print "\nBellaCoind has been installed in /usr/local/bin/\n";
	print "\nThe installer has finished.  Thank you for choosing BellaCoin!\nIf you have any questions, please visit http://www.bellacoin.com\nor e-mail support\@bellacoin.com\n\n";
      }
    }
  } else {
    print "\nError summary:\n\n";
    foreach my $error (@errors) {
      print "Error installing: $error\n";
    }
    print "\nPlease copy the above errors and mail them to installer\@bellacoin.com\n";
  }

  }
} else {
  die("Exiting the installer as per user request!\n");
}
