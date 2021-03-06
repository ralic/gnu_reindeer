SUBDIRS = libgnu
ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = \
	-I$(top_builddir) -I$(top_srcdir) $(LTDLINCL) \
	-I$(top_srcdir)/libgnu -I$(top_builddir)/libgnu \
	$(GLIB_CFLAGS) \
	-DAC_renlibdir=\"$(renlibdir)\"
AM_LDFLAGS =
AM_XSLTPROC_FLAGS = \
	--xinclude \
	--path /usr/share/xml/docbook/stylesheet/docbook-xsl

BUILT_SOURCES =
EXTRA_DIST =
CLEANFILES =
MOSTLYCLEANFILES =

include_HEADERS =
noinst_LTLIBRARIES =
lib_LTLIBRARIES =
EXTRA_LTLIBRARIES =

renlib_LTLIBRARIES =

vapidir = $(datadir)/vala/vapi
dist_vapi_DATA =

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA =

EXTRA_DIST += m4/gnulib-cache.m4

dist-hook: gen-ChangeLog

.PHONY: gen-ChangeLog
gen-ChangeLog:
	if test -d $(top_srcdir)/.git; then \
		(cd $(top_srcdir) && ./build-aux/gitlog-to-changelog) > $(distdir)/cl-t; \
		rm -f $(distdir)/ChangeLog; \
		mv $(distdir)/cl-t $(distdir)/ChangeLog; \
	fi

#
# libltdl
#

include libltdl/Makefile.inc

#
# libreindeer
#

lib_LTLIBRARIES += reindeer/libreindeer-0.2.la

reindeer_libreindeer_0_2_la_SOURCES = \
	reindeer/reindeer.c reindeer/reindeer.h \
	reindeer/backdata.c reindeer/backdata.h \
	reindeer/matrix.c reindeer/vector.c reindeer/color.c reindeer/datablock.c \
	reindeer/vectorarray.c reindeer/colorarray.c \
	reindeer/indexarray.c \
	reindeer/material.c reindeer/light.c \
	reindeer/template.c reindeer/template.h \
	reindeer/object.c reindeer/object.h

reindeer_libreindeer_0_2_la_LIBADD = \
	$(LTLIBOBJS) $(LIBLTDL) $(GLIB_LIBS) libgnu/libgnu.la

reindeer_libreindeer_0_2_la_LDFLAGS = \
	-export-dynamic -version-info 2:0:0

renincludedir = $(includedir)/reindeer-0.2/ren
reninclude_HEADERS = \
	ren/ren.h ren/types.h ren/base.h ren/version.h \
	ren/impl.h ren/tmpl.h ren/funcs.h \
	ren/matrix.h ren/vector.h ren/color.h ren/datablock.h \
	ren/vectorarray.h ren/colorarray.h \
	ren/indexarray.h \
	ren/material.h ren/light.h \
	ren/template.h ren/object.h

dist_vapi_DATA += \
	ren/reindeer-0.2.vapi \
	ren/reindeer-0.2.deps

pkgconfig_DATA += reindeer/reindeer-0.2.pc

#
# libreindeer-debug
#

renlib_LTLIBRARIES += debug/libreindeer-debug.0.2.0.la

debug_libreindeer_debug_0_2_0_la_SOURCES = \
	debug/debug.c

debug_libreindeer_debug_0_2_0_la_LIBADD = \
	$(LTLIBOBJS) reindeer/libreindeer-0.2.la

debug_libreindeer_debug_0_2_0_la_LDFLAGS = \
	-module -avoid-version

#
# documentation
#

reindeer_DOCBOOK = \
	doc/reindeer.docbook \
	doc/about.docbook \
	doc/obtaining.docbook \
	doc/compiling.docbook \
	doc/basic-example.docbook \
	doc/thread-safety.docbook \
	doc/types.docbook \
	doc/init-fini.docbook \
	doc/colors.docbook \
	doc/data-blocks.docbook \
	doc/matrices.docbook \
	doc/vectors.docbook \
	doc/buffers.docbook \
	doc/transformation.docbook \
	doc/vector-arrays.docbook \
	doc/color-arrays.docbook \
	doc/lights.docbook \
	doc/materials.docbook \
	doc/templates.docbook \
	doc/objects.docbook \
	doc/fdl-1.3.docbook

CLEANFILES += \
	doc/reindeer.html doc/html_node.stamp doc/html_node/*.html
EXTRA_DIST += \
	$(reindeer_DOCBOOK) \
	doc/common.xsl doc/html.xsl doc/html-chunk.xsl

if HAVE_DOCBOOK_XSLT
dist_html_DATA = doc/reindeer.html

htmlnodedir = $(htmldir)/html_node
dist_htmlnode_DATA = doc/html_node/*.html
endif

doc/reindeer.html: $(reindeer_DOCBOOK) $(srcdir)/doc/html.xsl
	$(AM_V_GEN)$(XSLTPROC) -o $@ $(AM_XSLTPROC_FLAGS) $(XSLTPROC_FLAGS) \
		--path /usr/share/xml/docbook/stylesheet/docbook-xsl/html \
		$(srcdir)/doc/html.xsl $(srcdir)/doc/reindeer.docbook

doc/html_node/*.html: doc/html_node.stamp

V_hn = $(V_hn_$(V))
V_hn_ = $(V_hn_$(AM_DEFAULT_VERBOSITY))
V_hn_0 = @echo "  GEN    doc/html_node";

doc/html_node.stamp: $(reindeer_DOCBOOK) $(srcdir)/doc/html-chunk.xsl
	$(V_hn)$(XSLTPROC) -o doc/html_node/ $(AM_XSLTPROC_FLAGS) $(XSLTPROC_FLAGS) \
		--path /usr/share/xml/docbook/stylesheet/docbook-xsl/html \
		$(srcdir)/doc/html-chunk.xsl $(srcdir)/doc/reindeer.docbook
	$(AM_V_at)touch $@
