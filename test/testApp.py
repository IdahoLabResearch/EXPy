"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""

import sys, os
projectRoot = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append((projectRoot))

from AppHandshakeProcessor import AppHandshakeProcessor
import json
from glob import glob
import binascii
import multiprocessing as mp

def ordered(obj):
    if isinstance(obj, dict):
        return sorted((k, ordered(v)) for k, v in obj.items())
    if isinstance(obj, list):
        return sorted(ordered(x) for x in obj)
    else:
        return obj
    
def test(packetName, jsonString, results):
    try:
        exiProcessor = AppHandshakeProcessor()
        exiString = exiProcessor.encode(jsonString)
        decodedJson = exiProcessor.decode(exiString)
        results.append((packetName, "Passed" if ordered(decodedJson) == ordered(jsonString) else "Failed", exiString))
    except Exception as e:
        results.append((packetName, "Failed", f"Error: {str(e)}"))

def main():
    inputs = []
    
    manager = mp.Manager()
    results = manager.list()

    for filePath in sorted(glob(projectRoot + "/test/AppHandshake/*.json")):

        packetName = os.path.basename(filePath).split(".")[0]
        jsonString = json.load(open(filePath))

        inputs.append((packetName, jsonString))

    procs = [mp.Process(target=test, args=(packetName, jsonString, results)) for packetName, jsonString in inputs]
    for proc in procs:
        proc.start()
    
    for i, proc in enumerate(procs):
        proc.join()
        if proc.exitcode != 0:
            packetName = inputs[i][0]
            existing_results = [r[0] for r in results]
            if packetName not in existing_results:
                results.append((packetName, "Failed", f"Process exited with code {proc.exitcode}"))


    print("=== AppHandshake Tests ===")
    print("Test results:")
    for packetName, isSuccess, exiString in sorted(results, key=lambda x: x[0]):
        if isSuccess == "Failed" and str(exiString).startswith("Error:"):
            print(f" - {packetName:<30}: {isSuccess:<6} {"--" if isSuccess == "Passed" else "=="} {exiString}")
        elif isSuccess == "Failed" and str(exiString).startswith("Process exited"):
            print(f" - {packetName:<30}: {isSuccess:<6} {"--" if isSuccess == "Passed" else "=="} {exiString}")
        else:
            print(f" - {packetName:<30}: {isSuccess:<6} {"--" if isSuccess == "Passed" else "=="} EXI: {binascii.hexlify(exiString).decode('utf-8') if exiString else 'None'}")

if __name__ == "__main__":
    main()