"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""


from typing import Any


def exi_unsigned_t(octets:bytearray, octets_count:int)->dict[str, Any]:
    """
    :param octets: bytearray
    :param octets_count: uint64/unsignedLong
    """

    j = {
        "octets": octets,
        "octets_count": octets_count
    }

    return j

def exi_signed_t(data:dict[str, Any], is_negative:int)->dict[str, Any]:
    """
    :param data: dict[exi_unsigned_t]
    :param is_negative: uint8/unsignedByte
    """

    j = {
        "data": data,
        "is_negative": is_negative
    }

    return j