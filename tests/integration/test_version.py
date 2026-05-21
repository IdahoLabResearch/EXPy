def test_expy_exposes_version():
    import expy

    assert expy.__version__ == "1.0.0"


def test_v2gjson_exposes_version():
    from expy import v2gjson

    assert v2gjson.__version__ == "1.0.0"


def test_version_is_single_source():
    import expy
    from expy import v2gjson

    assert expy.__version__ is v2gjson.__version__
