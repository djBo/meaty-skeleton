# meaty-skeleton

This repo is forked from the [original](https://gitlab.com/sortie/meaty-skeleton/-/commit/084d1624bedaa9f9e395f055c6bd99299bd97f58) as explained in the [main article](https://wiki.osdev.org/Meaty_Skeleton).




### Goal
After following a large number of tutorials on building your own "kernel", I figured it was time to do some of the stuff mentioned [here](https://wiki.osdev.org/Meaty_Skeleton#Moving_Forward). Not only do I want to learn, I also want to apply some of the things they teach on the Meaty Skeleton template. The destination is more about gaining knowledge than about creating an operating system.

At a minimum I'd like to implement some Arduino like behavior, where a single program is executed in a loop. The idea rose after experimenting with a single ``program.c`` file containing only ``setup()`` and ``loop()``. The goal here is to learn how to compile, load and execute that program dynamically from let's say an initrd, so you don't have to recompile the entire kernel as I did in the experiments.

Other interesting idea's came to mind on deciding a minimum set of "devices" to support. This derives directly from my breadboarded 6502 machine supported by a Teensy 4.0. Implementing these items gives our program a *generic* playground allowing creativity to take over. It basically comes down to supporting at least:

* Visual output  
  Be it a simple terminal, a tiny 64x48 pixel OLED screen or a **x86 frame buffer** at 800x600x32.
* Human input  
  The all important **keyboard** and optionally mouse, or just a bunch of switches and dials.
* Audible output  
  The simplest being a **PC speaker**, but maybe an R-2R ladder or an actual sound card.
* Randomness  
  The quality and source (hardware or **code**) is less important, but always useful to create dynamic behavior.
* Timing  
  A rudimentary form of timing (**PIT**/HPET or other hardware timers) and possibly date/time (DS1337 or CMOS).
* Buses  
  I2C, SPI, CAN, **USB**, ISA, PCI, PCIe and so many more.
* Storage  
  Hard drives, floppies, magnetic tape, SD cards... you name it...

At a maximum I'd like to perform the above in user mode, with ``program.c`` being compiled against our libc. This is a fairly large goal, since it touches and requires some of the more complex elements to implement like system calls and probably requires proper paging and tasking. This possibly includes things like memory management & multi-core support, making it even harder. This is obviously a long term goal, and is akin to *nice to have*. However, most of this **is** very interesting to learn regardless of how hard it seems at first.




### Changes

* Replace the scripts with a single Makefile  
  One of the goals in moving forward is improving the build system.

* Adding multiboot.h from [grub master](https://git.savannah.gnu.org/git/grub.git)  
  A minor modification was needed to remove the multiboot_apm_info struct at the bottom as it uses unknown grub_* types.

* Adding inportb/outportb functions and extend tty  
  The tty code comes straight from [JamesM's tutorial](http://www.jamesmolloy.co.uk/tutorial_html/).

* Implement abs/itoa and extend printf  
  This allows us to use %d and %h for decimal and hex respectivly.

* Add the x86 specific GDT/IDT structures and ISR/IRQ support  
  The main code comes from [BrandonF's tutorial](http://www.osdever.net/bkerndev/Docs/title.htm) with some additions from JamesM.

* Add a minimal timer at 100Hz  
  Again from BrandonF, with additional fixes mentioned on the [BrandonF Known Bugs](https://wiki.osdev.org/Bran%27s_Kernel_Development_Tutorial_Known_Bugs) page.

* Add rand/srand  
  Code comes from the [Standard's Example](https://wiki.osdev.org/Random_Number_Generator#The_Standard.27s_Example).

* Add minimal keyboard support and implement getchar  
  More from BrandonF, with minor additions.

* Replace the poor-man's printf with something more [substantial](https://www.menie.org/georges/embedded/printf-stdarg.c)




### Targets
The root Makefile replaces the original build scripts into specific targets.

| Target | Deps | Description |
|--|--|--|
| all | iso qemu | Builds everything and runs qemu |
| headers | - | Installs kernel headers in sysroot |
| build | headers | Builds the kernel |
| iso | build | Builds the iso image |
| qemu | genesis.iso | Run qemu with the genesis.iso image |
| clean | - | Cleans up all build artifacts |
| reset | - | Resets file permissions, useful i.c.w. WSL |
| docker | - | Build the cross-compiler for Ubuntu 18.04 |




### Docker
After succesfully following the [cross compiler tutorial](https://wiki.osdev.org/GCC_Cross-Compiler) by the letter on Ubuntu 18.04, it was time to dockerize it. The cross-compiler is created when you build the image. The cross-compiler can be used on WSL after installing libmpc-dev and libmpfr-dev.

Building the image: ``docker build -t genesis .``  
Running the image: ``docker run -it --rm --name genesis genesis``  
You can now use the cross compiler which is installed in ``/opt/cross``.

**build.sh**  
To turn the image into a builder, adjust the last line of the Dockerfile to be:

```
FROM target-build
```

The limited build.sh extends the image to use parameters:

```
docker run -it --rm --name genesis genesis <git-repo> <make-target> [branch]
```

Example to build this code:

```
docker run -it --rm --name genesis genesis https://github.com/djBo/meaty-skeleton.git build genesis
```




### Todo

* Paging  
  We are at the point, where paging might become usefull. Also, I need to understand it properly. This also includes stuff like the heap, virtual memory etc.

* Frambuffer Support  
  Already written in a small side project with some fun demo's in 800x600x32. Also includes a very minimal character font and extends terminal capabilities.

* VFS / initrd  
  JamesM has a nice example which I'd like to follow. Most importantly, his ``make_initrd.c`` actually compiles.

* PC Speaker Support  
  Already written in a small side project with a simple beep demo and tested on actual hardware.

* User-mode/tasking etc  
  This is a large chapter, including task switching and system calls etc. There is so much I need to learn before I understand this.

* Hardware rand/srand  
  Already written in a small side project with a demo showing the vast improvement over the existing implementation.

* Learn to "load a program"  
  After usermode/tasking etc. it is time to actually create a program executable, store in on the initrd, load it into ram, and execute it.

* Transform the keyboard code into a "driver"  
  Again, something I need to learn how to do.


* Add mouse support  
  This probably requires minimal USB support too.

* Multi-core support  
  Extending the tasking mechanism to support more than 1 cpu to distribute processing load.

* Learn from others  
  So many [projects](https://wiki.osdev.org/Projects), so much code to look at. Some could be realy usefull in learning any of the above and possibly even more.




### Known Bugs

* Problem #12 on the [JamesM Known Bugs](https://wiki.osdev.org/James_Molloy%27s_Tutorial_Known_Bugs) page  
  ISR 17 and 21 have error codes. Both Brandon and James have implemented these as no-error, and has been copied as is.
