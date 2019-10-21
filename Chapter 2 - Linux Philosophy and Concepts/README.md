# Linux History

## History
### Introduction
Linux is an open source computer operating system, initially developed on and for Intel
x86-based personal computers. It has been subsequently ported to an astoundingly long list
of other hardware platforms, from tiny embedded appliances to the world's largest
supercomputers.

### Linux History
Linus Torvalds was a student in Helsinki, Finland, in 1991, when he started a project:
writing his own operating system kernel. He also collected together and/or developed the
other essential ingredients required to construct an entire operating system with his
kernel at the center. It wasn't long before this became known as the Linux kernel. 

In 1992, Linux was re-licensed using the General Public License (GPL) by GNU (a project of
the Free Software Foundation or FSF, which promotes freely available software), which made
it possible to build a worldwide community of developers. By combining the kernel with
other system components from the GNU project, numerous other developers created complete
systems called Linux distributions in the mid-90’s.

### More about Linux History
The Linux distributions created in the mid-90s provided the basis for fully free (in the
sense of freedom, not zero cost) computing and became a driving force in the open source
software movement. In 1998, major companies like IBM and Oracle announced their support for
the Linux platform and began major development efforts as well.

Today, Linux powers more than half of the servers on the Internet, the majority of
smartphones (via the Android system, which is built on top of Linux), and all of the
world’s most powerful supercomputers.

## Philosophy
### Introduction
Linux is constantly enhanced and maintained by a network of developers from all over the world collaborating over the Internet, with Linus Torvalds at the head. Technical skill, a desire to contribute, and the ability to collaborate with others are the only qualifications for participating.

### Linux Philosophy
Linux borrows heavily from the well-established UNIX operating system. It was written to be
a free and open source system to be used in place of UNIX, which at the time was designed
for computers much more powerful than PCs and was quite expensive. Files are stored in a
hierarchical filesystem, with the top node of the system being the root or simply "/".
Whenever possible, Linux makes its components available via files or objects that look like
files. Processes, devices, and network sockets are all represented by file-like objects,
and can often be worked with using the same utilities used for regular files. Linux is a
fully multitasking (i.e. multiple threads of execution are performed simultaneously),
multiuser operating system, with built-in networking and service processes known as daemons
in the UNIX world.

Note: Linux was inspired by UNIX, but it is not UNIX. 

## Community
### Introduction
There are many ways to engage with the Linux community:

* Post queries on relevant discussion forums.
* Subscribe to discussion threads.
* Join local Linux groups that meet in your area.

### More about Linux Community
The Linux community is a far-reaching ecosystem consisting of developers, system administrators, users and vendors who use many different forums to connect with one
another. Among the most popular are:

* Internet Relay Chat (IRC) software (such as WeeChat, HexChat, Pidgin and XChat)
* Online communities and discussion boards including Linux User Groups (both local and
online)
* Many collaborative projects hosted on services such as GitHub
* Newsgroups and mailing lists, including the Linux Kernel Mailing List
* Community events, e.g. Hackathons, Install Fests, Open Source Summits and Embedded Linux
Conferences.

A portal to one of the most powerful online user communities can be found at [linux.com](linux.com).
This site is hosted by The Linux Foundation and serves over one million unique visitors
every month. It has active sections on:

* News
* Community discussion threads
* Free tutorials and user tips.

## Terminology
###Introduction
When you start exploring Linux, you will soon come across some terms which may be
unfamiliar, such as distribution, boot loader, desktop environment, etc. Before we proceed
further, let's stop and take a look at some basic terminology used in Linux to help you get
up to speed.

### Linux terminology
1. **Kernel**
..* The kernel is considered the brain of the Linux operating system. It controls hardware and makes the hardware interact with applications.
..* Examples: Red Hat Enterprise Linux, Fedora, Ubuntu and Gentoo.
2. **Boot loader**
..* Program that boots the operating system.
..* Examples: BRUB and ISOLINUX.
3. **Service**
..* Program that runs as a background process
..* Examples: httpd, nfsd, ntpd, ftpd and named.
4. **Filesystem**
..* Method for storing and organizing files
..* Examples: ext3, ext4, FAT, XFS and Btrfs.
5. **X Window System** 
..* Provides the standard toolkit and protocol to build graphical user interfaces on nearly all Linux systems.
6. **Desktop Environment**
..* Graphical user interface on top of the operating system
..* Examples: GNOME, KDE, Xfce and Fluxbox.
7. **Command Line**
..* Interface for typing commands on top of the operating system.
8. **Shell**
..* Command line interpreter that interprets the commnad line input and instructs the operating system to perform any necessary tasks and commands.
..* Examples: bash, tcsh and zsh.

## Distribution
### Introduction
Suppose you have been assigned to a project building a product for a Linux platform.
Project requirements include making sure the project works properly on the most widely
used Linux distributions. To accomplish this, you need to learn about the different
components, services, and configurations associated with each distribution. We are about
to look at how you would go about doing exactly that.

### Linux Distributions
So, what is a Linux distribution and how does it relate to the Linux kernel?

The Linux kernel is the core of the operating system. A full Linux distribution consists
of the kernel plus a number of other software tools for file-related operations, user
management, and software package management. Each of these tools provides a part of the
complete system. Each tool is often its own separate project, with its own developers
working to perfect that piece of the system.

Examples of other essential tools and ingredients provided by distributions include the C
C++ compiler, the gdb debugger, the core system libraries applications need to link with
in order to run, the low-level interface for drawing graphics on the screen, as well as
the higher-level desktop environment, and the system for installing and updating the
various components, including the kernel itself.  And all distributions come with a rather
complete suite of applications already installed.

![](distroroles.png)
Distribution Roles

## Services Associated with Distributions
The vast variety of Linux distributions are designed to cater to many different audiences
and organizations, according to their specific needs and tastes. However, large
organizations, such as companies and governmental institutions and other entities, tend to
choose the major commercially-supported distributions from Red Hat, SUSE, and Canonical 
Ubuntu).

CentOS is a popular free alternative to Red Hat Enterprise Linux (RHEL) and is often used
by organizations that are comfortable operating without paid technical support. Ubuntu and
Fedora are widely used by developers and are also popular in the educational realm.
Scientific Linux is favored by the scientific research community for its compatibility
with scientific and mathematical software packages. Both CentOS and Scientific Linux are
binary-compatible with RHEL; i.e. in most cases, binary software packages will install
properly across the distributions.

Many commercial distributors, including Red Hat, Ubuntu, SUSE, and Oracle, provide long
term fee-based support for their distributions, as well as hardware and software
certification. All major distributors provide update services for keeping your system
primed with the latest security and bug fixes, and performance enhancements, as well as
provide online support resources.

![](LFS01_ch02_screen_24.png)
Services Associated with Distributions
