#!@PYTHON_EXECUTABLE@
#ckwg +4
# Copyright 2011-2013 by Kitware, Inc. All Rights Reserved. Please refer to
# KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
# Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.


def test_import():
    try:
        import sprokit.pipeline.edge
    except:
        test_error("Failed to import the edge module")


def test_create():
    from sprokit.pipeline import config
    from sprokit.pipeline import edge

    c = config.empty_config()

    edge.Edge()
    edge.Edge(c)
    edge.Edges()


def test_datum_create():
    from sprokit.pipeline import datum
    from sprokit.pipeline import edge
    from sprokit.pipeline import stamp

    d = datum.complete()
    s = stamp.new_stamp(1)

    edge.EdgeDatum()
    edge.EdgeDatum(d, s)
    edge.EdgeData()


def test_api_calls():
    from sprokit.pipeline import config
    from sprokit.pipeline import datum
    from sprokit.pipeline import edge
    from sprokit.pipeline import modules
    from sprokit.pipeline import process
    from sprokit.pipeline import process_registry
    from sprokit.pipeline import stamp

    e = edge.Edge()

    e.makes_dependency()
    e.has_data()
    e.full_of_data()
    e.datum_count()

    d = datum.complete()
    s = stamp.new_stamp(1)

    ed = edge.EdgeDatum(d, s)

    e.push_datum(ed)
    e.get_datum()

    e.push_datum(ed)
    e.peek_datum()
    e.pop_datum()

    modules.load_known_modules()

    reg = process_registry.ProcessRegistry.self()

    p = reg.create_process('orphan', process.ProcessName())

    e.set_upstream_process(p)
    e.set_downstream_process(p)

    e.mark_downstream_as_complete()
    e.is_downstream_complete()

    e.config_dependency
    e.config_capacity


def test_datum_api_calls():
    from sprokit.pipeline import datum
    from sprokit.pipeline import edge
    from sprokit.pipeline import stamp

    d = datum.complete()
    s = stamp.new_stamp(1)

    ed = edge.EdgeDatum(d, s)

    ed.datum
    ed.datum = d
    ed.stamp
    ed.stamp = s


if __name__ == '__main__':
    import os
    import sys

    if not len(sys.argv) == 4:
        test_error("Expected three arguments")
        sys.exit(1)

    testname = sys.argv[1]

    os.chdir(sys.argv[2])

    sys.path.append(sys.argv[3])

    tests = \
        { 'import': test_import
        , 'create': test_create
        , 'datum_create': test_datum_create
        , 'api_calls': test_api_calls
        , 'datum_api_calls': test_datum_api_calls
        }

    from sprokit.test.test import *

    run_test(testname, tests)