#!/usr/bin/env python
#ckwg +5
# Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
# KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
# Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.


def log(msg):
    import sys
    sys.stderr.write("%s\n" % msg)


def test_import():
    try:
        import vistk.pipeline.datum
    except:
        log("Error: Failed to import the datum module")


def test_empty():
    from vistk.pipeline import datum

    d = datum.empty()

    if not d.type() == datum.DatumType.EMPTY:
        log("Error: Datum type mismatch")

    if len(d.get_error()):
        log("Error: A empty datum has an error string")

    got_exception = False

    try:
        d.get_datum()
    except:
        got_exception = True

    if not got_exception:
        log("Error: Did not get exception when retrieving data from a empty datum")


def test_complete():
    from vistk.pipeline import datum

    d = datum.complete()

    if not d.type() == datum.DatumType.COMPLETE:
        log("Error: Datum type mismatch")

    if len(d.get_error()):
        log("Error: A complete datum has an error string")

    got_exception = False

    try:
        d.get_datum()
    except:
        got_exception = True

    if not got_exception:
        log("Error: Did not get exception when retrieving data from a complete datum")


def test_error():
    from vistk.pipeline import datum

    err = 'An error'

    d = datum.error(err)

    if not d.type() == datum.DatumType.ERROR:
        log("Error: Datum type mismatch")

    if not d.get_error() == err:
        log("Error: An error datum did not keep the message")

    got_exception = False

    try:
        d.get_datum()
    except:
        got_exception = True

    if not got_exception:
        log("Error: Did not get exception when retrieving data from a error datum")


def main(testname):
    if testname == 'import':
        test_import()
    elif testname == 'empty':
        test_empty()
    elif testname == 'complete':
        test_complete()
    elif testname == 'error':
        test_error()
    else:
        log("Error: No such test '%s'" % testname)


if __name__ == '__main__':
    import os
    import sys

    if not len(sys.argv) == 4:
        log("Error: Expected three arguments")
        sys.exit(1)

    testname = sys.argv[1]

    os.chdir(sys.argv[2])

    sys.path.append(sys.argv[3])

    main(testname)