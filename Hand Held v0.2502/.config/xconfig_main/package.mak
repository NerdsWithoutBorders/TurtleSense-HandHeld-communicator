#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#

unexport MAKEFILE_LIST
MK_NOGENDEPS := $(filter clean,$(MAKECMDGOALS))
override PKGDIR = xconfig_main
XDCINCS = -I. -I$(strip $(subst ;, -I,$(subst $(space),\$(space),$(XPKGPATH))))
XDCCFGDIR = package/cfg/

#
# The following dependencies ensure package.mak is rebuilt
# in the event that some included BOM script changes.
#
ifneq (clean,$(MAKECMDGOALS))
C:/ti/xdctools_3_25_04_88/packages/xdc/utils.js:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/utils.js
C:/ti/xdctools_3_25_04_88/packages/xdc/xdc.tci:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/xdc.tci
C:/ti/xdctools_3_25_04_88/packages/xdc/template.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/template.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/om2.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/om2.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/xmlgen.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/xmlgen.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/xmlgen2.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/xmlgen2.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/Warnings.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/Warnings.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/IPackage.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/IPackage.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/package.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/package.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/services/global/Clock.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/services/global/Clock.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/services/global/Trace.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/services/global/Trace.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/bld.js:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/bld.js
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/BuildEnvironment.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/BuildEnvironment.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/PackageContents.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/PackageContents.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/_gen.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/_gen.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Library.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Library.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Executable.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Executable.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Repository.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Repository.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Configuration.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Configuration.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Script.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Script.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Manifest.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Manifest.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Utils.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/Utils.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/ITarget.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/ITarget.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/ITarget2.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/ITarget2.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/ITarget3.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/ITarget3.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/ITargetFilter.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/ITargetFilter.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/bld/package.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/bld/package.xs
package.mak: config.bld
C:/ti/xdctools_3_25_04_88/packages/ti/targets/ITarget.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/ti/targets/ITarget.xs
C:/ti/xdctools_3_25_04_88/packages/ti/targets/C28_large.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/ti/targets/C28_large.xs
C:/ti/xdctools_3_25_04_88/packages/ti/targets/C28_float.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/ti/targets/C28_float.xs
C:/ti/xdctools_3_25_04_88/packages/ti/targets/package.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/ti/targets/package.xs
C:/ti/grace_2_20_02_32/packages/ti/targets/msp430/elf/MSP430.xs:
package.mak: C:/ti/grace_2_20_02_32/packages/ti/targets/msp430/elf/MSP430.xs
C:/ti/grace_2_20_02_32/packages/ti/targets/msp430/elf/MSP430X.xs:
package.mak: C:/ti/grace_2_20_02_32/packages/ti/targets/msp430/elf/MSP430X.xs
C:/ti/grace_2_20_02_32/packages/ti/targets/msp430/elf/MSP430X_small.xs:
package.mak: C:/ti/grace_2_20_02_32/packages/ti/targets/msp430/elf/MSP430X_small.xs
C:/ti/grace_2_20_02_32/packages/ti/targets/msp430/elf/package.xs:
package.mak: C:/ti/grace_2_20_02_32/packages/ti/targets/msp430/elf/package.xs
package.mak: package.bld
C:/ti/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/compiler.opt.xdt:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/compiler.opt.xdt
C:/ti/xdctools_3_25_04_88/packages/xdc/services/io/File.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/services/io/File.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/services/io/package.xs:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/services/io/package.xs
C:/ti/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/compiler.defs.xdt:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/compiler.defs.xdt
C:/ti/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/custom.mak.exe.xdt:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/custom.mak.exe.xdt
C:/ti/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/package.xs.xdt:
package.mak: C:/ti/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/package.xs.xdt
endif

ti.targets.msp430.elf.MSP430X.rootDir ?= C:/ti/ccsv5/tools/compiler/msp430_4.2.3
ti.targets.msp430.elf.packageBase ?= C:/ti/grace_2_20_02_32/packages/ti/targets/msp430/elf/
.PRECIOUS: $(XDCCFGDIR)/%.oe430X
.PHONY: all,e430X .dlls,e430X .executables,e430X test,e430X
all,e430X: .executables,e430X
.executables,e430X: .libraries,e430X
.executables,e430X: .dlls,e430X
.dlls,e430X: .libraries,e430X
.libraries,e430X: .interfaces
	@$(RM) $@
	@$(TOUCH) "$@"

.help::
	@$(ECHO) xdc test,e430X
	@$(ECHO) xdc .executables,e430X
	@$(ECHO) xdc .libraries,e430X
	@$(ECHO) xdc .dlls,e430X


all: .executables 
.executables: .libraries .dlls
.libraries: .interfaces

PKGCFGS := $(wildcard package.xs) package/build.cfg
.interfaces: package/package.xdc.inc package/package.defs.h package.xdc $(PKGCFGS)

-include package/package.xdc.dep
package/%.xdc.inc package/%_xconfig_main.c package/%.defs.h: %.xdc $(PKGCFGS)
	@$(MSG) generating interfaces for package xconfig_main" (because $@ is older than $(firstword $?))" ...
	$(XSRUN) -f xdc/services/intern/cmd/build.xs $(MK_IDLOPTS) -m package/package.xdc.dep -i package/package.xdc.inc package.xdc

.dlls,e430X .dlls: main.pe430X

-include package/cfg/main_pe430X.mak
-include package/cfg/main_pe430X.cfg.mak
ifeq (,$(MK_NOGENDEPS))
-include package/cfg/main_pe430X.dep
endif
main.pe430X: package/cfg/main_pe430X.xdl
	@


ifeq (,$(wildcard .libraries,e430X))
main.pe430X package/cfg/main_pe430X.c: .libraries,e430X
endif

package/cfg/main_pe430X.c package/cfg/main_pe430X.h package/cfg/main_pe430X.xdl: override _PROG_NAME := main.xe430X
package/cfg/main_pe430X.c package/cfg/main_pe430X.xdl: override _PROG_XSOPTS = -Dxdc.cfg.tsort.policy=fast -Dxdc.cfg.gen.metadataFiles=false -Dxdc.cfg.SourceDir.verbose=7
package/cfg/main_pe430X.c: package/cfg/main_pe430X.cfg

clean:: clean,e430X
	-$(RM) package/cfg/main_pe430X.cfg
	-$(RM) package/cfg/main_pe430X.dep
	-$(RM) package/cfg/main_pe430X.c
	-$(RM) package/cfg/main_pe430X.xdc.inc

clean,e430X::
	-$(RM) main.pe430X
.executables,e430X .executables: main.xe430X

main.xe430X: |main.pe430X

-include package/cfg/main.xe430X.mak
main.xe430X: package/cfg/main_pe430X.oe430X 
	$(RM) $@
	@$(MSG) lnke430X $@ ...
	$(RM) $(XDCCFGDIR)/$@.map
	$(ti.targets.msp430.elf.MSP430X.rootDir)/bin/lnk430 -w -q -u _c_int00 -fs $(XDCCFGDIR)$(dir $@).  -q -o $@ package/cfg/main_pe430X.oe430X   package/cfg/main_pe430X.xdl  -c -m $(XDCCFGDIR)/$@.map -l $(ti.targets.msp430.elf.MSP430X.rootDir)/lib/libc.a
	
main.xe430X: export C_DIR=
main.xe430X: PATH:=$(ti.targets.msp430.elf.MSP430X.rootDir)/bin/;$(PATH)
main.xe430X: Path:=$(ti.targets.msp430.elf.MSP430X.rootDir)/bin/;$(PATH)

main.test test,e430X test: main.xe430X.test

main.xe430X.test:: main.xe430X
ifeq (,$(_TESTLEVEL))
	@$(MAKE) -R -r --no-print-directory -f $(XDCROOT)/packages/xdc/bld/xdc.mak _TESTLEVEL=1 main.xe430X.test
else
	@$(MSG) running $<  ...
	$(call EXEC.main.xe430X, ) 
endif

clean,e430X::
	-$(RM) .tmp,main.xe430X,0,*


clean:: clean,e430X

clean,e430X::
	-$(RM) main.xe430X
clean:: 
	-$(RM) package/cfg/main_pe430X.pjt
%,copy:
	@$(if $<,,$(MSG) don\'t know how to build $*; exit 1)
	@$(MSG) cp $< $@
	$(RM) $@
	$(CP) $< $@
main_pe430X.oe430X,copy : package/cfg/main_pe430X.oe430X
main_pe430X.se430X,copy : package/cfg/main_pe430X.se430X

$(XDCCFGDIR)%.c $(XDCCFGDIR)%.h $(XDCCFGDIR)%.xdl: $(XDCCFGDIR)%.cfg .interfaces $(XDCROOT)/packages/xdc/cfg/Main.xs
	@$(MSG) "configuring $(_PROG_NAME) from $< ..."
	$(CONFIG) $(_PROG_XSOPTS) xdc.cfg $(_PROG_NAME) $(XDCCFGDIR)$*.cfg $(XDCCFGDIR)$*

.PHONY: release,xconfig_main
xconfig_main.tar: package/package.bld.xml
xconfig_main.tar: package/build.cfg
xconfig_main.tar: package/package.xdc.inc
ifeq (,$(MK_NOGENDEPS))
-include package/rel/xconfig_main.tar.dep
endif
package/rel/xconfig_main/xconfig_main/package/package.rel.xml: .force
	@$(MSG) generating external release references $@ ...
	$(XS) $(JSENV) -f $(XDCROOT)/packages/xdc/bld/rel.js $(MK_RELOPTS) . $@

xconfig_main.tar: package/rel/xconfig_main.xdc.inc package/rel/xconfig_main/xconfig_main/package/package.rel.xml
	@$(MSG) making release file $@ "(because of $(firstword $?))" ...
	-$(RM) $@
	$(call MKRELTAR,package/rel/xconfig_main.xdc.inc,package/rel/xconfig_main.tar.dep)


release release,xconfig_main: all xconfig_main.tar
clean:: .clean
	-$(RM) xconfig_main.tar
	-$(RM) package/rel/xconfig_main.xdc.inc
	-$(RM) package/rel/xconfig_main.tar.dep

clean:: .clean
	-$(RM) .libraries .libraries,*
clean:: 
	-$(RM) .dlls .dlls,*
#
# The following clean rule removes user specified
# generated files or directories.
#

ifneq (clean,$(MAKECMDGOALS))
ifeq (,$(wildcard package))
    $(shell $(MKDIR) package)
endif
ifeq (,$(wildcard package/cfg))
    $(shell $(MKDIR) package/cfg)
endif
ifeq (,$(wildcard package/lib))
    $(shell $(MKDIR) package/lib)
endif
ifeq (,$(wildcard package/rel))
    $(shell $(MKDIR) package/rel)
endif
ifeq (,$(wildcard package/internal))
    $(shell $(MKDIR) package/internal)
endif
endif
clean::
	-$(RMDIR) package

include custom.mak
clean:: 
	-$(RM) package/xconfig_main.pjt
