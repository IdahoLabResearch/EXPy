def test_exiprocessor_exposes_version():
    import EXIProcessor

    assert EXIProcessor.__version__ == "1.0.0"


def test_v2gjson_exposes_version():
    import V2Gjson

    assert V2Gjson.__version__ == "1.0.0"


def test_version_is_single_source():
    import EXIProcessor
    import V2Gjson

    assert EXIProcessor.__version__ is V2Gjson.__version__
