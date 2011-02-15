#!/bin/sh
set -e
export TAR_OPTIONS="--owner root --group root --mode a+rX"
pwd=`pwd`
version="$1"
if [ -z "$version" ]
then
    printf 'Usage: %s <version>\n' "$0"
    exit 1
fi
cd "`dirname "$0"`/../"
tmpdir=`mktemp -d get-orig-source.XXXXXX`
uscan --noconf --force-download --rename --download-version="$version" --destdir="$tmpdir"
cd "$tmpdir"
tar -xjf cuneiform_*.orig.tar.bz2
rm *.tar.bz2
# Remove documentation without source:
rm -Rf cuneiform-*/cuneiform_src/Addfiles/
rm -Rf cuneiform-*/cuneiform_src/Kern/icrashreport/
mv cuneiform-*/ "cuneiform-$version.orig"
tar -cjf "$pwd/cuneiform_$version+dfsg.orig.tar.bz2" cuneiform-*.orig/
cd ..
rm -Rf "$tmpdir"

# vim:ts=4 sw=4 et
