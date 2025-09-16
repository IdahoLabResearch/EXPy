/*
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
*/

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <new>

#include <cbv2g/iso_2/iso2_msgDefDecoder.h>
#include <cbv2g/iso_2/iso2_msgDefEncoder.h>
#include <cbv2g/iso_2/iso2_msgDefDatatypes.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <cxxopts.hpp>

#include <iostream>
#include <fstream>
using namespace std;

extern "C" {
    const char* decodeFromPythonBytes(const uint8_t* data, size_t size) {
        iso2_exiDocument outDoc;
        exi_bitstream_t inEXI;

        json outJson;

        exi_bitstream_init(&inEXI, const_cast<uint8_t*>(data), size, 0, nullptr);
        decode_iso2_exiDocument(&inEXI, &outDoc);

        const auto& header = outDoc.V2G_Message.Header;
        const auto& body = outDoc.V2G_Message.Body;

        outJson["Header"]["SessionID"]["bytesLen"] = header.SessionID.bytesLen;
        for (int i = 0; i < header.SessionID.bytesLen; i++) {
            outJson["Header"]["SessionID"]["bytes"][i] = header.SessionID.bytes[i];
        }

        if (header.Notification_isUsed) {
            outJson["Header"]["Notification"]["FaultCode"] = header.Notification.FaultCode;
            outJson["Header"]["Notification"]["FaultMsg"]["charactersLen"] = header.Notification.FaultMsg.charactersLen;
            for (int i = 0; i < header.Notification.FaultMsg.charactersLen; i++) {
                outJson["Header"]["Notification"]["FaultMsg"][i] = header.Notification.FaultMsg.characters[i];
            }
        }

        // TODO: implement signature, check if same as DIN header and copy over if so

        if (body.AuthorizationReq_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            if (body.AuthorizationReq.Id_isUsed) {
                outJson["Body"]["AuthorizationReq"]["Id"]["charactersLen"] = body.AuthorizationReq.Id.charactersLen;
                for (int i = 0; i < body.AuthorizationReq.Id.charactersLen; i++) {
                    outJson["Body"]["AuthorizationReq"]["Id"]["characters"][i] = body.AuthorizationReq.Id.characters[i];
                }
            }

            if (body.AuthorizationReq.GenChallenge_isUsed) {
                outJson["Body"]["AuthorizationReq"]["GenChallenge"]["bytesLen"] = body.AuthorizationReq.GenChallenge.bytesLen;
                for (int i = 0; i < body.AuthorizationReq.GenChallenge.bytesLen; i++) {
                    outJson["Body"]["AuthorizationReq"]["GenChallenge"]["bytes"][i] = body.AuthorizationReq.GenChallenge.bytes[i];
                }
            } 
            
            if (!(body.AuthorizationReq.GenChallenge_isUsed | body.AuthorizationReq.Id_isUsed)) {
                outJson["Body"]["AuthorizationReq"]["isUsed"] = true;
            }

        } else if (body.AuthorizationRes_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["AuthorizationRes"]["ResponseCode"] = body.AuthorizationRes.ResponseCode;

            outJson["Body"]["AuthorizationRes"]["EVSEProcessing"] = body.AuthorizationRes.EVSEProcessing;

        } else if (body.CableCheckReq_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVReady"] = body.CableCheckReq.DC_EVStatus.EVReady;

            outJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVErrorCode"] = body.CableCheckReq.DC_EVStatus.EVErrorCode;

            outJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVRESSSOC"] = body.CableCheckReq.DC_EVStatus.EVRESSSOC;

        } else if (body.CableCheckRes_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["CableCheckRes"]["ResponseCode"] = body.CableCheckRes.ResponseCode;

            outJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.CableCheckRes.DC_EVSEStatus.NotificationMaxDelay;
            if (body.CableCheckRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed) {
                outJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.CableCheckRes.DC_EVSEStatus.EVSEIsolationStatus;
            }
            outJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.CableCheckRes.DC_EVSEStatus.EVSEStatusCode;

            outJson["Body"]["CableCheckRes"]["EVSEProcessing"] = body.CableCheckRes.EVSEProcessing;

        } else if (body.CertificateInstallationReq_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["CertificateInstallationReq"]["Id"]["charactersLen"] = body.CertificateInstallationReq.Id.charactersLen;
            for (int i = 0; i < body.CertificateInstallationReq.Id.charactersLen; i++) {
                outJson["Body"]["CertificateInstallationReq"]["Id"]["characters"][i] = body.CertificateInstallationReq.Id.characters[i];
            }

            outJson["Body"]["CertificateInstallationReq"]["OEMProvisioningCert"]["bytesLen"] = body.CertificateInstallationReq.OEMProvisioningCert.bytesLen;
            for (int i = 0; i < body.CertificateInstallationReq.OEMProvisioningCert.bytesLen; i++) {
                outJson["Body"]["CertificateInstallationReq"]["OEMProvisioningCert"]["bytes"][i] = body.CertificateInstallationReq.OEMProvisioningCert.bytes[i];
            }

            outJson["Body"]["CertificateInstallationReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["arrayLen"] = body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.arrayLen;
            for (int i = 0; i < body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.arrayLen; i++) {
                outJson["Body"]["CertificateInstallationReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509IssuerName"]["charactersLen"] = body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509IssuerName.charactersLen;
                for (int j = 0; j < body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509IssuerName.charactersLen; j++) {
                    outJson["Body"]["CertificateInstallationReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509IssuerName"]["characters"][j] = body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509IssuerName.characters[j];
                }
                outJson["Body"]["CertificateInstallationReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509SerialNumber"]["data"]["octets_count"] = body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.data.octets_count;
                for (size_t k = 0; k < body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.data.octets_count; k++) {
                    outJson["Body"]["CertificateInstallationReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509SerialNumber"]["data"]["octets"][k] = body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.data.octets[k];
                }
                outJson["Body"]["CertificateInstallationReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509SerialNumber"]["is_negative"] = body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.is_negative;
            }

        } else if (body.CertificateInstallationRes_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["CertificateInstallationRes"]["ResponseCode"] = body.CertificateInstallationRes.ResponseCode;

            if (body.CertificateInstallationRes.SAProvisioningCertificateChain.Id_isUsed) {
                outJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["Id"]["charactersLen"] = body.CertificateInstallationRes.SAProvisioningCertificateChain.Id.charactersLen;
                for (int i = 0; i < body.CertificateInstallationRes.SAProvisioningCertificateChain.Id.charactersLen; i++) {
                    outJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["Id"]["characters"][i] = body.CertificateInstallationRes.SAProvisioningCertificateChain.Id.characters[i];
                }
            }
            outJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["Certificate"]["bytesLen"] = body.CertificateInstallationRes.SAProvisioningCertificateChain.Certificate.bytesLen;
            for (int i = 0; i < body.CertificateInstallationRes.SAProvisioningCertificateChain.Certificate.bytesLen; i++) {
                outJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["Certificate"]["bytes"][i] = body.CertificateInstallationRes.SAProvisioningCertificateChain.Certificate.bytes[i];
            }
            if (body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates_isUsed) {
                outJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["SubCertificates"]["Certificate"]["arrayLen"] = body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates.Certificate.arrayLen;
                for (int i = 0; i < body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates.Certificate.arrayLen; i++) {
                    outJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["SubCertificates"]["Certificate"]["array"][i]["bytesLen"] = body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates.Certificate.array[i].bytesLen;
                    for (int j = 0; j < body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates.Certificate.array[i].bytesLen; j++) {
                        outJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["SubCertificates"]["Certificate"]["array"][i]["bytes"][j] = body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates.Certificate.array[i].bytes[j];
                    }
                }
            }

            if (body.CertificateInstallationRes.ContractSignatureCertChain.Id_isUsed) {
                outJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["Id"]["charactersLen"] = body.CertificateInstallationRes.ContractSignatureCertChain.Id.charactersLen;
                for (int i = 0; i < body.CertificateInstallationRes.ContractSignatureCertChain.Id.charactersLen; i++) {
                    outJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["Id"]["characters"][i] = body.CertificateInstallationRes.ContractSignatureCertChain.Id.characters[i];
                }
            }
            outJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["Certificate"]["bytesLen"] = body.CertificateInstallationRes.ContractSignatureCertChain.Certificate.bytesLen;
            for (int i = 0; i < body.CertificateInstallationRes.ContractSignatureCertChain.Certificate.bytesLen; i++) {
                outJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["Certificate"]["bytes"][i] = body.CertificateInstallationRes.ContractSignatureCertChain.Certificate.bytes[i];
            }
            if (body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates_isUsed) {
                outJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["arrayLen"] = body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen;
                for (int i = 0; i < body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen; i++) {
                    outJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytesLen"] = body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen;
                    for (int j = 0; j < body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen; j++) {
                        outJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytes"][j] = body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytes[j];
                    }
                }
            }

            outJson["Body"]["CertificateInstallationRes"]["ContractSignatureEncryptedPrivateKey"]["Id"]["charactersLen"] = body.CertificateInstallationRes.ContractSignatureEncryptedPrivateKey.Id.charactersLen;
            for (int i = 0; i < body.CertificateInstallationRes.ContractSignatureEncryptedPrivateKey.Id.charactersLen; i++) {
                outJson["Body"]["CertificateInstallationRes"]["ContractSignatureEncryptedPrivateKey"]["Id"]["characters"][i] = body.CertificateInstallationRes.ContractSignatureEncryptedPrivateKey.Id.characters[i];
            }
            outJson["Body"]["CertificateInstallationRes"]["ContractSignatureEncryptedPrivateKey"]["CONTENT"]["bytesLen"] = body.CertificateInstallationRes.ContractSignatureEncryptedPrivateKey.CONTENT.bytesLen;
            for (int i = 0; i < body.CertificateInstallationRes.ContractSignatureEncryptedPrivateKey.CONTENT.bytesLen; i++) {
                outJson["Body"]["CertificateInstallationRes"]["ContractSignatureEncryptedPrivateKey"]["CONTENT"]["bytes"][i] = body.CertificateInstallationRes.ContractSignatureEncryptedPrivateKey.CONTENT.bytes[i];
            }

            outJson["Body"]["CertificateInstallationRes"]["DHpublickey"]["Id"]["charactersLen"] = body.CertificateInstallationRes.DHpublickey.Id.charactersLen;
            for (int i = 0; i < body.CertificateInstallationRes.DHpublickey.Id.charactersLen; i++) {
                outJson["Body"]["CertificateInstallationRes"]["DHpublickey"]["Id"]["characters"][i] = body.CertificateInstallationRes.DHpublickey.Id.characters[i];
            }
            outJson["Body"]["CertificateInstallationRes"]["DHpublickey"]["CONTENT"]["bytesLen"] = body.CertificateInstallationRes.DHpublickey.CONTENT.bytesLen;
            for (int i = 0; i < body.CertificateInstallationRes.DHpublickey.CONTENT.bytesLen; i++) {
                outJson["Body"]["CertificateInstallationRes"]["DHpublickey"]["CONTENT"]["bytes"][i] = body.CertificateInstallationRes.DHpublickey.CONTENT.bytes[i];
            }

            outJson["Body"]["CertificateInstallationRes"]["eMAID"]["Id"]["charactersLen"] = body.CertificateInstallationRes.eMAID.Id.charactersLen;
            for (int i = 0; i < body.CertificateInstallationRes.eMAID.Id.charactersLen; i++) {
                outJson["Body"]["CertificateInstallationRes"]["eMAID"]["Id"]["characters"][i] = body.CertificateInstallationRes.eMAID.Id.characters[i];
            }
            outJson["Body"]["CertificateInstallationRes"]["eMAID"]["CONTENT"]["charactersLen"] = body.CertificateInstallationRes.eMAID.CONTENT.charactersLen;
            for (int i = 0; i < body.CertificateInstallationRes.eMAID.CONTENT.charactersLen; i++) {
                outJson["Body"]["CertificateInstallationRes"]["eMAID"]["CONTENT"]["characters"][i] = body.CertificateInstallationRes.eMAID.CONTENT.characters[i];
            }

        } else if (body.CertificateUpdateReq_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            outJson["Body"]["CertificateUpdateReq"]["Id"]["charactersLen"] = body.CertificateUpdateReq.Id.charactersLen;
            for (int i = 0; i < body.CertificateUpdateReq.Id.charactersLen; i++) {
                outJson["Body"]["CertificateUpdateReq"]["Id"]["characters"][i] = body.CertificateUpdateReq.Id.characters[i];
            }

            outJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["Id"]["charactersLen"] = body.CertificateUpdateReq.ContractSignatureCertChain.Id.charactersLen;
            for (int i = 0; i < body.CertificateUpdateReq.ContractSignatureCertChain.Id.charactersLen; i++) {
                outJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["Id"]["characters"][i] = body.CertificateUpdateReq.ContractSignatureCertChain.Id.characters[i];
            }
            outJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["Certificate"]["bytesLen"] = body.CertificateUpdateReq.ContractSignatureCertChain.Certificate.bytesLen;
            for (int i = 0; i < body.CertificateUpdateReq.ContractSignatureCertChain.Certificate.bytesLen; i++) {
                outJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["Certificate"]["bytes"][i] = body.CertificateUpdateReq.ContractSignatureCertChain.Certificate.bytes[i];
            }
            if (body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates_isUsed) {
                outJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["arrayLen"] = body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen;
                for (int i = 0; i < body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen; i++) {
                    outJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytesLen"] = body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen;
                    for (int j = 0; j < body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen; j++) {
                        outJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytes"][j] = body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytes[j];
                    }
                }
            }

            outJson["Body"]["CertificateUpdateReq"]["eMAID"]["charactersLen"] = body.CertificateUpdateReq.eMAID.charactersLen;
            for (int i = 0; i < body.CertificateUpdateReq.eMAID.charactersLen; i++) {
                outJson["Body"]["CertificateUpdateReq"]["eMAID"]["characters"][i] = body.CertificateUpdateReq.eMAID.characters[i];
            }

            outJson["Body"]["CertificateUpdateReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["arrayLen"] = body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.arrayLen;
            for (int i = 0; i < body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.arrayLen; i++) {
                outJson["Body"]["CertificateUpdateReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509IssuerName"]["charactersLen"] = body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509IssuerName.charactersLen;
                for (int j = 0; j < body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509IssuerName.charactersLen; j++) {
                    outJson["Body"]["CertificateUpdateReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509IssuerName"]["characters"][j] = body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509IssuerName.characters[j];
                }
                outJson["Body"]["CertificateUpdateReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509SerialNumber"]["data"]["octets_count"] = body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.data.octets_count;
                for (size_t k = 0; k < body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.data.octets_count; k++) {
                    outJson["Body"]["CertificateUpdateReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509SerialNumber"]["data"]["octets"][k] = body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.data.octets[k];
                }
                outJson["Body"]["CertificateUpdateReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509SerialNumber"]["is_negative"] = body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.is_negative;
            }

        } else if (body.CertificateUpdateRes_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["CertificateUpdateRes"]["ResponseCode"] = body.CertificateUpdateRes.ResponseCode;

            if (body.CertificateUpdateRes.SAProvisioningCertificateChain.Id_isUsed) {
                outJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["Id"]["charactersLen"] = body.CertificateUpdateRes.SAProvisioningCertificateChain.Id.charactersLen;
                for (int i = 0; i < body.CertificateUpdateRes.SAProvisioningCertificateChain.Id.charactersLen; i++) {
                    outJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["Id"]["characters"][i] = body.CertificateUpdateRes.SAProvisioningCertificateChain.Id.characters[i];
                }
            }
            outJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["Certificate"]["bytesLen"] = body.CertificateUpdateRes.SAProvisioningCertificateChain.Certificate.bytesLen;
            for (int i = 0; i < body.CertificateUpdateRes.SAProvisioningCertificateChain.Certificate.bytesLen; i++) {
                outJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["Certificate"]["bytes"][i] = body.CertificateUpdateRes.SAProvisioningCertificateChain.Certificate.bytes[i];
            }
            if (body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates_isUsed) {
                outJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["SubCertificates"]["Certificate"]["arrayLen"] = body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates.Certificate.arrayLen;
                for (int i = 0; i < body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates.Certificate.arrayLen; i++) {
                    outJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["SubCertificates"]["Certificate"]["array"][i]["bytesLen"] = body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates.Certificate.array[i].bytesLen;
                    for (int j = 0; j < body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates.Certificate.array[i].bytesLen; j++) {
                        outJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["SubCertificates"]["Certificate"]["array"][i]["bytes"][j] = body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates.Certificate.array[i].bytes[j];
                    }
                }
            }

            if (body.CertificateUpdateRes.ContractSignatureCertChain.Id_isUsed) {
                outJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["Id"]["charactersLen"] = body.CertificateUpdateRes.ContractSignatureCertChain.Id.charactersLen;
                for (int i = 0; i < body.CertificateUpdateRes.ContractSignatureCertChain.Id.charactersLen; i++) {
                    outJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["Id"]["characters"][i] = body.CertificateUpdateRes.ContractSignatureCertChain.Id.characters[i];
                }
            }
            outJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["Certificate"]["bytesLen"] = body.CertificateUpdateRes.ContractSignatureCertChain.Certificate.bytesLen;
            for (int i = 0; i < body.CertificateUpdateRes.ContractSignatureCertChain.Certificate.bytesLen; i++) {
                outJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["Certificate"]["bytes"][i] = body.CertificateUpdateRes.ContractSignatureCertChain.Certificate.bytes[i];
            }
            if (body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates_isUsed) {
                outJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["arrayLen"] = body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen;
                for (int i = 0; i < body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen; i++) {
                    outJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytesLen"] = body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen;
                    for (int j = 0; j < body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen; j++) {
                        outJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytes"][j] = body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytes[j];
                    }
                }
            }

            outJson["Body"]["CertificateUpdateRes"]["ContractSignatureEncryptedPrivateKey"]["Id"]["charactersLen"] = body.CertificateUpdateRes.ContractSignatureEncryptedPrivateKey.Id.charactersLen;
            for (int i = 0; i < body.CertificateUpdateRes.ContractSignatureEncryptedPrivateKey.Id.charactersLen; i++) {
                outJson["Body"]["CertificateUpdateRes"]["ContractSignatureEncryptedPrivateKey"]["Id"]["characters"][i] = body.CertificateUpdateRes.ContractSignatureEncryptedPrivateKey.Id.characters[i];
            }
            outJson["Body"]["CertificateUpdateRes"]["ContractSignatureEncryptedPrivateKey"]["CONTENT"]["bytesLen"] = body.CertificateUpdateRes.ContractSignatureEncryptedPrivateKey.CONTENT.bytesLen;
            for (int i = 0; i < body.CertificateUpdateRes.ContractSignatureEncryptedPrivateKey.CONTENT.bytesLen; i++) {
                outJson["Body"]["CertificateUpdateRes"]["ContractSignatureEncryptedPrivateKey"]["CONTENT"]["bytes"][i] = body.CertificateUpdateRes.ContractSignatureEncryptedPrivateKey.CONTENT.bytes[i];
            }

            outJson["Body"]["CertificateUpdateRes"]["DHpublickey"]["Id"]["charactersLen"] = body.CertificateUpdateRes.DHpublickey.Id.charactersLen;
            for (int i = 0; i < body.CertificateUpdateRes.DHpublickey.Id.charactersLen; i++) {
                outJson["Body"]["CertificateUpdateRes"]["DHpublickey"]["Id"]["characters"][i] = body.CertificateUpdateRes.DHpublickey.Id.characters[i];
            }
            outJson["Body"]["CertificateUpdateRes"]["DHpublickey"]["CONTENT"]["bytesLen"] = body.CertificateUpdateRes.DHpublickey.CONTENT.bytesLen;
            for (int i = 0; i < body.CertificateUpdateRes.DHpublickey.CONTENT.bytesLen; i++) {
                outJson["Body"]["CertificateUpdateRes"]["DHpublickey"]["CONTENT"]["bytes"][i] = body.CertificateUpdateRes.DHpublickey.CONTENT.bytes[i];
            }

            outJson["Body"]["CertificateUpdateRes"]["eMAID"]["Id"]["charactersLen"] = body.CertificateUpdateRes.eMAID.Id.charactersLen;
            for (int i = 0; i < body.CertificateUpdateRes.eMAID.Id.charactersLen; i++) {
                outJson["Body"]["CertificateUpdateRes"]["eMAID"]["Id"]["characters"][i] = body.CertificateUpdateRes.eMAID.Id.characters[i];
            }
            outJson["Body"]["CertificateUpdateRes"]["eMAID"]["CONTENT"]["charactersLen"] = body.CertificateUpdateRes.eMAID.CONTENT.charactersLen;
            for (int i = 0; i < body.CertificateUpdateRes.eMAID.CONTENT.charactersLen; i++) {
                outJson["Body"]["CertificateUpdateRes"]["eMAID"]["CONTENT"]["characters"][i] = body.CertificateUpdateRes.eMAID.CONTENT.characters[i];
            }

            if (body.CertificateUpdateRes.RetryCounter_isUsed) {
                outJson["Body"]["CertificateUpdateRes"]["RetryCounter"] = body.CertificateUpdateRes.RetryCounter;
            }

        } else if (body.ChargeParameterDiscoveryReq_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            if (body.ChargeParameterDiscoveryReq.MaxEntriesSAScheduleTuple_isUsed) {
                outJson["Body"]["ChargeParameterDiscoveryReq"]["MaxEntriesSAScheduleTuple"] = body.ChargeParameterDiscoveryReq.MaxEntriesSAScheduleTuple;
            }

            outJson["Body"]["ChargeParameterDiscoveryReq"]["RequestedEnergyTransferMode"] = body.ChargeParameterDiscoveryReq.RequestedEnergyTransferMode;

            if (body.ChargeParameterDiscoveryReq.AC_EVChargeParameter_isUsed) {
                if (body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.DepartureTime_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["DepartureTime"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.DepartureTime;
                }

                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EAmount"]["Multiplier"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EAmount.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EAmount"]["Unit"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EAmount.Unit;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EAmount"]["Value"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EAmount.Value;

                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Multiplier"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxVoltage.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Unit"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxVoltage.Unit;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Value"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxVoltage.Value;

                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Multiplier"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxCurrent.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Unit"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxCurrent.Unit;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Value"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxCurrent.Value;

                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMinCurrent"]["Multiplier"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMinCurrent.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMinCurrent"]["Unit"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMinCurrent.Unit;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMinCurrent"]["Value"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMinCurrent.Value;
            }

            if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter_isUsed) {
                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DepartureTime_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DepartureTime"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DepartureTime;
                }
                
                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVReady"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVReady;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVErrorCode"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVErrorCode;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVRESSSOC"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVRESSSOC;

                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Multiplier"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Unit"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Unit;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Value"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Value;

                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Multiplier"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Multiplier;
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Unit"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Unit;
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Value"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Value;
                }

                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Multiplier"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Unit"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Unit;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Value"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Value;

                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Multiplier"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Multiplier;
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Unit"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Unit;
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Value"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Value;
                }

                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.FullSOC_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["FullSOC"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.FullSOC;
                }

                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.BulkSOC_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["BulkSOC"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.BulkSOC;
                }
            }

            if (body.ChargeParameterDiscoveryReq.EVChargeParameter_isUsed) {
                if (body.ChargeParameterDiscoveryReq.EVChargeParameter.DepartureTime_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DepartureTime"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DepartureTime;
                }

                if (body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.DepartureTime_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["DepartureTime"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.DepartureTime;
                }
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EAmount"]["Multiplier"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EAmount.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EAmount"]["Unit"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EAmount.Unit;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EAmount"]["Value"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EAmount.Value;

                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Multiplier"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMaxVoltage.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Unit"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMaxVoltage.Unit;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Value"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMaxVoltage.Value;

                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Multiplier"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMaxCurrent.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Unit"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMaxCurrent.Unit;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Value"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMaxCurrent.Value;

                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMinCurrent"]["Multiplier"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMinCurrent.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMinCurrent"]["Unit"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMinCurrent.Unit;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMinCurrent"]["Value"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMinCurrent.Value;

                if (body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.DepartureTime_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["DepartureTime"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.DepartureTime;
                }
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVReady"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.DC_EVStatus.EVReady;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVErrorCode"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.DC_EVStatus.EVErrorCode;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVRESSSOC"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.DC_EVStatus.EVRESSSOC;

                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Multiplier"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumCurrentLimit.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Unit"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumCurrentLimit.Unit;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Value"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumCurrentLimit.Value;

                if (body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumPowerLimit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Multiplier"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumPowerLimit.Multiplier;
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Unit"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumPowerLimit.Unit;
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Value"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumPowerLimit.Value;
                }

                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Multiplier"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumVoltageLimit.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Unit"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumVoltageLimit.Unit;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Value"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumVoltageLimit.Value;

                if (body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVEnergyCapacity_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Multiplier"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVEnergyCapacity.Multiplier;
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Unit"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVEnergyCapacity.Unit;
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Value"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVEnergyCapacity.Value;
                }

                if (body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.FullSOC_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["FullSOC"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.FullSOC;
                }

                if (body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.BulkSOC_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["BulkSOC"] = body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.BulkSOC;
                }
            }

        } else if (body.ChargeParameterDiscoveryRes_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["ChargeParameterDiscoveryRes"]["ResponseCode"] = body.ChargeParameterDiscoveryRes.ResponseCode;
            outJson["Body"]["ChargeParameterDiscoveryRes"]["EVSEProcessing"] = body.ChargeParameterDiscoveryRes.EVSEProcessing;

            if (body.ChargeParameterDiscoveryRes.SAScheduleList_isUsed) {
                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["arrayLen"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.arrayLen;
                for (int i = 0; i < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.arrayLen; i++) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SAScheduleTupleID"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SAScheduleTupleID;
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["arrayLen"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.arrayLen;
                    for (int j = 0; j < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.arrayLen; j++) {
                        if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval_isUsed) {
                            outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["RelativeTimeInterval"]["start"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.start;
                            if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.duration_isUsed) {
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["RelativeTimeInterval"]["duration"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.duration;
                            }
                        }
                        if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].TimeInterval_isUsed) {
                            outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["TimeInterval"]["_unused"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].TimeInterval._unused;
                        }
                        outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["PMax"]["Multiplier"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].PMax.Multiplier;
                        outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["PMax"]["Unit"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].PMax.Unit;
                        outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["PMax"]["Value"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].PMax.Value;
                    }
                    if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff_isUsed) {
                        if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id_isUsed) {
                            outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["Id"]["charactersLen"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id.charactersLen;
                            for (int k = 0; k < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id.charactersLen; k++) {
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["Id"]["characters"][k] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id.characters[k];
                            }
                        }
                        outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffID"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffID;
                        if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription_isUsed) {
                            outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffDescription"]["charactersLen"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription.charactersLen;
                            for (int k = 0; k < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription.charactersLen; k++) {
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffDescription"]["characters"][k] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription.characters[k];
                            }
                        }
                        if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.NumEPriceLevels_isUsed) {
                            outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["NumEPriceLevels"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.NumEPriceLevels;
                        }
                        outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["arrayLen"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.arrayLen;
                        for (int j = 0; j < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.arrayLen; j++) {
                            if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval_isUsed) {
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["RelativeTimeInterval"]["start"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval.start;
                                if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval.duration_isUsed) {
                                    outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["RelativeTimeInterval"]["duration"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval.duration;
                                }
                            }
                            if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].TimeInterval_isUsed) {
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["TimeInterval"]["_unused"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].TimeInterval._unused;
                            }
                            if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].EPriceLevel_isUsed) {
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["EPriceLevel"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].EPriceLevel;
                            }
                            outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["arrayLen"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.arrayLen;
                            for (int k = 0; k < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.arrayLen; k++) {
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["startValue"]["Multiplier"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].startValue.Multiplier;
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["startValue"]["Unit"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].startValue.Unit;
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["startValue"]["Value"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].startValue.Value;

                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["Cost"]["arrayLen"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.arrayLen;
                                for (int l = 0; l < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.arrayLen; l++) {
                                    outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["Cost"]["array"][l]["costKind"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.array[l].costKind;
                                    outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["Cost"]["array"][l]["amount"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.array[l].amount;
                                    if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.array[l].amountMultiplier_isUsed) {
                                        outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["Cost"]["array"][l]["amountMultiplier"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.array[l].amountMultiplier;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (body.ChargeParameterDiscoveryRes.SASchedules_isUsed) {
                outJson["Body"]["ChargeParameterDiscoveryRes"]["SASchedules"]["_unused"] = body.ChargeParameterDiscoveryRes.SASchedules._unused;
            }

            if (body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter_isUsed) {
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["NotificationMaxDelay"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["EVSENotification"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.EVSENotification;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["RCD"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.RCD;

                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSENominalVoltage"]["Multiplier"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSENominalVoltage.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSENominalVoltage"]["Unit"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSENominalVoltage.Unit;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSENominalVoltage"]["Value"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSENominalVoltage.Value;

                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"]["Multiplier"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"]["Unit"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Unit;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"]["Value"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Value;
            }

            if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter_isUsed) {
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["EVSENotification"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSENotification;
                if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEIsolationStatus_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEIsolationStatus;
                }
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEStatusCode;

                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Unit;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Value;

                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Unit;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Value;

                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Unit;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Value;

                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Unit;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Value;

                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Unit;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Value;

                if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Multiplier;
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Unit;
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Value;
                }

                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Multiplier;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Unit;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Value;

                if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Multiplier;
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Unit;
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Value;
                }
            }

            if (body.ChargeParameterDiscoveryRes.EVSEChargeParameter_isUsed) {
                outJson["Body"]["ChargeParameterDiscoveryRes"]["EVSEChargeParameter"]["_unused"] = body.ChargeParameterDiscoveryRes.EVSEChargeParameter._unused;
            }
        } else if (body.ChargingStatusReq_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["ChargingStatusReq"]["_unused"] = body.ChargingStatusReq._unused;

        } else if (body.ChargingStatusRes_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["ChargingStatusRes"]["ResponseCode"] = body.ChargingStatusRes.ResponseCode;
            
            outJson["Body"]["ChargingStatusRes"]["EVSEID"]["charactersLen"] = body.ChargingStatusRes.EVSEID.charactersLen;
            for (int i = 0; i < body.ChargingStatusRes.EVSEID.charactersLen; i++) {
                outJson["Body"]["ChargingStatusRes"]["EVSEID"]["characters"][i] = body.ChargingStatusRes.EVSEID.characters[i];
            }

            if (body.ChargingStatusRes.EVSEMaxCurrent_isUsed) {
                outJson["Body"]["ChargingStatusRes"]["EVSEMaxCurrent"]["Multiplier"] = body.ChargingStatusRes.EVSEMaxCurrent.Multiplier;
                outJson["Body"]["ChargingStatusRes"]["EVSEMaxCurrent"]["Unit"] = body.ChargingStatusRes.EVSEMaxCurrent.Unit;
                outJson["Body"]["ChargingStatusRes"]["EVSEMaxCurrent"]["Value"] = body.ChargingStatusRes.EVSEMaxCurrent.Value;
            }

            if (body.ChargingStatusRes.MeterInfo_isUsed) {
                outJson["Body"]["ChargingStatusRes"]["MeterInfo"]["MeterID"]["charactersLen"] = body.ChargingStatusRes.MeterInfo.MeterID.charactersLen;
                for (int i = 0; i < body.ChargingStatusRes.MeterInfo.MeterID.charactersLen; i++) {
                    outJson["Body"]["ChargingStatusRes"]["MeterInfo"]["MeterID"]["characters"][i] = body.ChargingStatusRes.MeterInfo.MeterID.characters[i];
                }
                
                if (body.ChargingStatusRes.MeterInfo.MeterReading_isUsed) {
                    outJson["Body"]["ChargingStatusRes"]["MeterInfo"]["MeterReading"] = body.ChargingStatusRes.MeterInfo.MeterReading;
                }

                if (body.ChargingStatusRes.MeterInfo.SigMeterReading_isUsed) {
                    outJson["Body"]["ChargingStatusRes"]["MeterInfo"]["SigMeterReading"]["bytesLen"] = body.ChargingStatusRes.MeterInfo.SigMeterReading.bytesLen;
                    for (int i = 0; i < body.ChargingStatusRes.MeterInfo.SigMeterReading.bytesLen; i++) {
                        outJson["Body"]["ChargingStatusRes"]["MeterInfo"]["SigMeterReading"]["bytes"][i] = body.ChargingStatusRes.MeterInfo.SigMeterReading.bytes[i];
                    }
                }

                if (body.ChargingStatusRes.MeterInfo.MeterStatus_isUsed) {
                    outJson["Body"]["ChargingStatusRes"]["MeterInfo"]["MeterStatus"] = body.ChargingStatusRes.MeterInfo.MeterStatus;
                }

                if (body.ChargingStatusRes.MeterInfo.TMeter_isUsed) {
                    outJson["Body"]["ChargingStatusRes"]["MeterInfo"]["TMeter"] = body.ChargingStatusRes.MeterInfo.TMeter;
                }
            }

            if (body.ChargingStatusRes.ReceiptRequired_isUsed) {
                outJson["Body"]["ChargingStatusRes"]["ReceiptRequired"] = body.ChargingStatusRes.ReceiptRequired;
            }

            outJson["Body"]["ChargingStatusRes"]["AC_EVSEStatus"]["NotificationMaxDelay"] = body.ChargingStatusRes.AC_EVSEStatus.NotificationMaxDelay;
            outJson["Body"]["ChargingStatusRes"]["AC_EVSEStatus"]["EVSENotification"] = body.ChargingStatusRes.AC_EVSEStatus.EVSENotification;
            outJson["Body"]["ChargingStatusRes"]["AC_EVSEStatus"]["RCD"] = body.ChargingStatusRes.AC_EVSEStatus.RCD;

        } else if (body.CurrentDemandReq_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVReady"] = body.CurrentDemandReq.DC_EVStatus.EVReady;
            outJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVErrorCode"] = body.CurrentDemandReq.DC_EVStatus.EVErrorCode;
            outJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVRESSSOC"] = body.CurrentDemandReq.DC_EVStatus.EVRESSSOC;

            outJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"]["Multiplier"] = body.CurrentDemandReq.EVTargetCurrent.Multiplier;
            outJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"]["Unit"] = body.CurrentDemandReq.EVTargetCurrent.Unit;
            outJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"]["Value"] = body.CurrentDemandReq.EVTargetCurrent.Value;

            if (body.CurrentDemandReq.EVMaximumVoltageLimit_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"]["Multiplier"] = body.CurrentDemandReq.EVMaximumVoltageLimit.Multiplier;
                outJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"]["Unit"] = body.CurrentDemandReq.EVMaximumVoltageLimit.Unit;
                outJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"]["Value"] = body.CurrentDemandReq.EVMaximumVoltageLimit.Value;
            }

            if (body.CurrentDemandReq.EVMaximumCurrentLimit_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"]["Multiplier"] = body.CurrentDemandReq.EVMaximumCurrentLimit.Multiplier;
                outJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"]["Unit"] = body.CurrentDemandReq.EVMaximumCurrentLimit.Unit;
                outJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"]["Value"] = body.CurrentDemandReq.EVMaximumCurrentLimit.Value;
            }

            if (body.CurrentDemandReq.EVMaximumPowerLimit_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"]["Multiplier"] = body.CurrentDemandReq.EVMaximumPowerLimit.Multiplier;
                outJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"]["Unit"] = body.CurrentDemandReq.EVMaximumPowerLimit.Unit;
                outJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"]["Value"] = body.CurrentDemandReq.EVMaximumPowerLimit.Value;
            }

            if (body.CurrentDemandReq.BulkChargingComplete_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["BulkChargingComplete"] = body.CurrentDemandReq.BulkChargingComplete;
            }

            outJson["Body"]["CurrentDemandReq"]["ChargingComplete"] = body.CurrentDemandReq.ChargingComplete;

            if (body.CurrentDemandReq.RemainingTimeToFullSoC_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"]["Multiplier"] = body.CurrentDemandReq.RemainingTimeToFullSoC.Multiplier;
                outJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"]["Unit"] = body.CurrentDemandReq.RemainingTimeToFullSoC.Unit;
                outJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"]["Value"] = body.CurrentDemandReq.RemainingTimeToFullSoC.Value;
            }

            if (body.CurrentDemandReq.RemainingTimeToBulkSoC_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"]["Multiplier"] = body.CurrentDemandReq.RemainingTimeToBulkSoC.Multiplier;
                outJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"]["Unit"] = body.CurrentDemandReq.RemainingTimeToBulkSoC.Unit;
                outJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"]["Value"] = body.CurrentDemandReq.RemainingTimeToBulkSoC.Value;
            }

            outJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"]["Multiplier"] = body.CurrentDemandReq.EVTargetVoltage.Multiplier;

        } else if (body.CurrentDemandRes_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["CurrentDemandRes"]["ResponseCode"] = body.CurrentDemandRes.ResponseCode;

            outJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.CurrentDemandRes.DC_EVSEStatus.NotificationMaxDelay;
            outJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["EVSENotification"] = body.CurrentDemandRes.DC_EVSEStatus.EVSENotification;
            if (body.CurrentDemandRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed) {
                outJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.CurrentDemandRes.DC_EVSEStatus.EVSEIsolationStatus;
            }
            outJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.CurrentDemandRes.DC_EVSEStatus.EVSEStatusCode;

            outJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"]["Multiplier"] = body.CurrentDemandRes.EVSEPresentVoltage.Multiplier;
            outJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"]["Unit"] = body.CurrentDemandRes.EVSEPresentVoltage.Unit;
            outJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"]["Value"] = body.CurrentDemandRes.EVSEPresentVoltage.Value;

            outJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"]["Multiplier"] = body.CurrentDemandRes.EVSEPresentCurrent.Multiplier;
            outJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"]["Unit"] = body.CurrentDemandRes.EVSEPresentCurrent.Unit;
            outJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"]["Value"] = body.CurrentDemandRes.EVSEPresentCurrent.Value;

            outJson["Body"]["CurrentDemandRes"]["EVSECurrentLimitAchieved"] = body.CurrentDemandRes.EVSECurrentLimitAchieved;
            outJson["Body"]["CurrentDemandRes"]["EVSEVoltageLimitAchieved"] = body.CurrentDemandRes.EVSEVoltageLimitAchieved;
            outJson["Body"]["CurrentDemandRes"]["EVSEPowerLimitAchieved"] = body.CurrentDemandRes.EVSEPowerLimitAchieved;

            if (body.CurrentDemandRes.EVSEMaximumVoltageLimit_isUsed) {
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"]["Multiplier"] = body.CurrentDemandRes.EVSEMaximumVoltageLimit.Multiplier;
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"]["Unit"] = body.CurrentDemandRes.EVSEMaximumVoltageLimit.Unit;
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"]["Value"] = body.CurrentDemandRes.EVSEMaximumVoltageLimit.Value;
            }

            if (body.CurrentDemandRes.EVSEMaximumCurrentLimit_isUsed) {
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"]["Multiplier"] = body.CurrentDemandRes.EVSEMaximumCurrentLimit.Multiplier;
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"]["Unit"] = body.CurrentDemandRes.EVSEMaximumCurrentLimit.Unit;
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"]["Value"] = body.CurrentDemandRes.EVSEMaximumCurrentLimit.Value;
            }

            if (body.CurrentDemandRes.EVSEMaximumPowerLimit_isUsed) {
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"]["Multiplier"] = body.CurrentDemandRes.EVSEMaximumPowerLimit.Multiplier;
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"]["Unit"] = body.CurrentDemandRes.EVSEMaximumPowerLimit.Unit;
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"]["Value"] = body.CurrentDemandRes.EVSEMaximumPowerLimit.Value;
            }

            outJson["Body"]["CurrentDemandRes"]["EVSEID"]["charactersLen"] = body.CurrentDemandRes.EVSEID.charactersLen;
            for (int i = 0; i < body.CurrentDemandRes.EVSEID.charactersLen; i++) {
                outJson["Body"]["CurrentDemandRes"]["EVSEID"]["characters"][i] = body.CurrentDemandRes.EVSEID.characters[i];
            }

            outJson["Body"]["CurrentDemandRes"]["SAScheduleTupleID"] = body.CurrentDemandRes.SAScheduleTupleID;

            if (body.CurrentDemandRes.MeterInfo_isUsed) {
                outJson["Body"]["CurrentDemandRes"]["MeterInfo"]["MeterID"]["charactersLen"] = body.CurrentDemandRes.MeterInfo.MeterID.charactersLen;
                for (int i = 0; i < body.CurrentDemandRes.MeterInfo.MeterID.charactersLen; i++) {
                    outJson["Body"]["CurrentDemandRes"]["MeterInfo"]["MeterID"]["characters"][i] = body.CurrentDemandRes.MeterInfo.MeterID.characters[i];
                }
                
                if (body.CurrentDemandRes.MeterInfo.MeterReading_isUsed) {
                    outJson["Body"]["CurrentDemandRes"]["MeterInfo"]["MeterReading"] = body.CurrentDemandRes.MeterInfo.MeterReading;
                }

                if (body.CurrentDemandRes.MeterInfo.SigMeterReading_isUsed) {
                    outJson["Body"]["CurrentDemandRes"]["MeterInfo"]["SigMeterReading"]["bytesLen"] = body.CurrentDemandRes.MeterInfo.SigMeterReading.bytesLen;
                    for (int i = 0; i < body.CurrentDemandRes.MeterInfo.SigMeterReading.bytesLen; i++) {
                        outJson["Body"]["CurrentDemandRes"]["MeterInfo"]["SigMeterReading"]["bytes"][i] = body.CurrentDemandRes.MeterInfo.SigMeterReading.bytes[i];
                    }
                }

                if (body.CurrentDemandRes.MeterInfo.MeterStatus_isUsed) {
                    outJson["Body"]["CurrentDemandRes"]["MeterInfo"]["MeterStatus"] = body.CurrentDemandRes.MeterInfo.MeterStatus;
                }

                if (body.CurrentDemandRes.MeterInfo.TMeter_isUsed) {
                    outJson["Body"]["CurrentDemandRes"]["MeterInfo"]["TMeter"] = body.CurrentDemandRes.MeterInfo.TMeter;
                }
            }

            if (body.CurrentDemandRes.ReceiptRequired_isUsed) {
                outJson["Body"]["CurrentDemandRes"]["ReceiptRequired"] = body.CurrentDemandRes.ReceiptRequired;
            }

        } else if (body.MeteringReceiptReq_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            if (body.MeteringReceiptReq.Id_isUsed) {
                outJson["Body"]["MeteringReceiptReq"]["Id"]["charactersLen"] = body.MeteringReceiptReq.Id.charactersLen;
                for (int i = 0; i < body.MeteringReceiptReq.Id.charactersLen; i++) {
                    outJson["Body"]["MeteringReceiptReq"]["Id"]["characters"][i] = body.MeteringReceiptReq.Id.characters[i];
                }
            }

            outJson["Body"]["MeteringReceiptReq"]["SessionID"]["bytesLen"] = body.MeteringReceiptReq.SessionID.bytesLen;
            for (int i = 0; i < body.MeteringReceiptReq.SessionID.bytesLen; i++) {
                outJson["Body"]["MeteringReceiptReq"]["SessionID"]["bytes"][i] = body.MeteringReceiptReq.SessionID.bytes[i];
            }

            if (body.MeteringReceiptReq.SAScheduleTupleID_isUsed) {
                outJson["Body"]["MeteringReceiptReq"]["SAScheduleTupleID"] = body.MeteringReceiptReq.SAScheduleTupleID;
            }

            outJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["MeterID"]["charactersLen"] = body.MeteringReceiptReq.MeterInfo.MeterID.charactersLen;
            for (int i = 0; i < body.MeteringReceiptReq.MeterInfo.MeterID.charactersLen; i++) {
                outJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["MeterID"]["characters"][i] = body.MeteringReceiptReq.MeterInfo.MeterID.characters[i];
            }
            if (body.MeteringReceiptReq.MeterInfo.MeterReading_isUsed) {
                outJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["MeterReading"] = body.MeteringReceiptReq.MeterInfo.MeterReading;
            }
            if (body.MeteringReceiptReq.MeterInfo.SigMeterReading_isUsed) {
                outJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["SigMeterReading"]["bytesLen"] = body.MeteringReceiptReq.MeterInfo.SigMeterReading.bytesLen;
                for (int i = 0; i < body.MeteringReceiptReq.MeterInfo.SigMeterReading.bytesLen; i++) {
                    outJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["SigMeterReading"]["bytes"][i] = body.MeteringReceiptReq.MeterInfo.SigMeterReading.bytes[i];
                }
            }
            if (body.MeteringReceiptReq.MeterInfo.MeterStatus_isUsed) {
                outJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["MeterStatus"] = body.MeteringReceiptReq.MeterInfo.MeterStatus;
            }
            if (body.MeteringReceiptReq.MeterInfo.TMeter_isUsed) {
                outJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["TMeter"] = body.MeteringReceiptReq.MeterInfo.TMeter;
            }

        } else if (body.MeteringReceiptRes_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["MeteringReceiptRes"]["ResponseCode"] = body.MeteringReceiptRes.ResponseCode;

            if (body.MeteringReceiptRes.AC_EVSEStatus_isUsed) {
                outJson["Body"]["MeteringReceiptRes"]["AC_EVSEStatus"]["NotificationMaxDelay"] = body.MeteringReceiptRes.AC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["MeteringReceiptRes"]["AC_EVSEStatus"]["EVSENotification"] = body.MeteringReceiptRes.AC_EVSEStatus.EVSENotification;
                outJson["Body"]["MeteringReceiptRes"]["AC_EVSEStatus"]["RCD"] = body.MeteringReceiptRes.AC_EVSEStatus.RCD;
            }

            if (body.MeteringReceiptRes.DC_EVSEStatus_isUsed) {
                outJson["Body"]["MeteringReceiptRes"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.MeteringReceiptRes.DC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["MeteringReceiptRes"]["DC_EVSEStatus"]["EVSENotification"] = body.MeteringReceiptRes.DC_EVSEStatus.EVSENotification;
                if (body.MeteringReceiptRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed) {
                    outJson["Body"]["MeteringReceiptRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.MeteringReceiptRes.DC_EVSEStatus.EVSEIsolationStatus;
                }
                outJson["Body"]["MeteringReceiptRes"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.MeteringReceiptRes.DC_EVSEStatus.EVSEStatusCode;
            }

            if (body.MeteringReceiptRes.EVSEStatus_isUsed) {
                outJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["NotificationMaxDelay"] = body.MeteringReceiptRes.EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["EVSENotification"] = body.MeteringReceiptRes.EVSEStatus.EVSENotification;

                outJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["AC_EVSEStatus"]["NotificationMaxDelay"] = body.MeteringReceiptRes.EVSEStatus.AC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["AC_EVSEStatus"]["EVSENotification"] = body.MeteringReceiptRes.EVSEStatus.AC_EVSEStatus.EVSENotification;
                outJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["AC_EVSEStatus"]["RCD"] = body.MeteringReceiptRes.EVSEStatus.AC_EVSEStatus.RCD;

                outJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.MeteringReceiptRes.EVSEStatus.DC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["DC_EVSEStatus"]["EVSENotification"] = body.MeteringReceiptRes.EVSEStatus.DC_EVSEStatus.EVSENotification;
                if (body.MeteringReceiptRes.EVSEStatus.DC_EVSEStatus.EVSEIsolationStatus_isUsed) {
                    outJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.MeteringReceiptRes.EVSEStatus.DC_EVSEStatus.EVSEIsolationStatus;
                }
                outJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.MeteringReceiptRes.EVSEStatus.DC_EVSEStatus.EVSEStatusCode;
            }

        } else if (body.PaymentDetailsReq_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["PaymentDetailsReq"]["eMAID"]["charactersLen"] = body.PaymentDetailsReq.eMAID.charactersLen;
            for (int i = 0; i < body.PaymentDetailsReq.eMAID.charactersLen; i++) {
                outJson["Body"]["PaymentDetailsReq"]["eMAID"]["characters"][i] = body.PaymentDetailsReq.eMAID.characters[i];
            }

            if (body.PaymentDetailsReq.ContractSignatureCertChain.Id_isUsed) {
                outJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["Id"]["charactersLen"] = body.PaymentDetailsReq.ContractSignatureCertChain.Id.charactersLen;
                for (int i = 0; i < body.PaymentDetailsReq.ContractSignatureCertChain.Id.charactersLen; i++) {
                    outJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["Id"]["characters"][i] = body.PaymentDetailsReq.ContractSignatureCertChain.Id.characters[i];
                }
            }
            outJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["Certificate"]["bytesLen"] = body.PaymentDetailsReq.ContractSignatureCertChain.Certificate.bytesLen;
            for (int i = 0; i < body.PaymentDetailsReq.ContractSignatureCertChain.Certificate.bytesLen; i++) {
                outJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["Certificate"]["bytes"][i] = body.PaymentDetailsReq.ContractSignatureCertChain.Certificate.bytes[i];
            }

            if (body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates_isUsed) {
                outJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["arrayLen"] = body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen;
                for (int i = 0; i < body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen; i++) {
                    outJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytesLen"] = body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen;
                    for (int j = 0; j < body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen; j++) {
                        outJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytes"][j] = body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytes[j];
                    }
                }
            }

        } else if (body.PaymentDetailsRes_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["PaymentDetailsRes"]["ResponseCode"] = body.PaymentDetailsRes.ResponseCode;

            outJson["Body"]["PaymentDetailsRes"]["GenChallenge"]["bytesLen"] = body.PaymentDetailsRes.GenChallenge.bytesLen;
            for (int i = 0; i < body.PaymentDetailsRes.GenChallenge.bytesLen; i++) {
                outJson["Body"]["PaymentDetailsRes"]["GenChallenge"]["bytes"][i] = body.PaymentDetailsRes.GenChallenge.bytes[i];
            }

            outJson["Body"]["PaymentDetailsRes"]["EVSETimeStamp"] = body.PaymentDetailsRes.EVSETimeStamp;

        } else if (body.PaymentServiceSelectionReq_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["PaymentServiceSelectionReq"]["SelectedPaymentOption"] = body.PaymentServiceSelectionReq.SelectedPaymentOption;

            outJson["Body"]["PaymentServiceSelectionReq"]["SelectedServiceList"]["SelectedService"]["arrayLen"] = body.PaymentServiceSelectionReq.SelectedServiceList.SelectedService.arrayLen;
            for (int i = 0; i < body.PaymentServiceSelectionReq.SelectedServiceList.SelectedService.arrayLen; i++) {
                outJson["Body"]["PaymentServiceSelectionReq"]["SelectedServiceList"]["SelectedService"]["array"][i]["ServiceID"] = body.PaymentServiceSelectionReq.SelectedServiceList.SelectedService.array[i].ServiceID;
                if (body.PaymentServiceSelectionReq.SelectedServiceList.SelectedService.array[i].ParameterSetID_isUsed) {
                    outJson["Body"]["PaymentServiceSelectionReq"]["SelectedServiceList"]["SelectedService"]["array"][i]["ParameterSetID"] = body.PaymentServiceSelectionReq.SelectedServiceList.SelectedService.array[i].ParameterSetID;
                }
            }

        } else if (body.PaymentServiceSelectionRes_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["PaymentServiceSelectionRes"]["ResponseCode"] = body.PaymentServiceSelectionRes.ResponseCode;

        } else if (body.PowerDeliveryReq_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["PowerDeliveryReq"]["ChargeProgress"] = body.PowerDeliveryReq.ChargeProgress;

            outJson["Body"]["PowerDeliveryReq"]["SAScheduleTupleID"] = body.PowerDeliveryReq.SAScheduleTupleID;

            if (body.PowerDeliveryReq.ChargingProfile_isUsed) {
                outJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["arrayLen"] = body.PowerDeliveryReq.ChargingProfile.ProfileEntry.arrayLen;
                for (int i = 0; i < body.PowerDeliveryReq.ChargingProfile.ProfileEntry.arrayLen; i++) {
                    outJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryStart"] = body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryStart;

                    outJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryMaxPower"]["Multiplier"] = body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxPower.Multiplier;
                    outJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryMaxPower"]["Unit"] = body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxPower.Unit;
                    outJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryMaxPower"]["Value"] = body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxPower.Value;

                    if (body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxNumberOfPhasesInUse_isUsed) {
                        outJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryMaxNumberOfPhasesInUse"] = body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxNumberOfPhasesInUse;
                    }
                }
            }

            if (body.PowerDeliveryReq.DC_EVPowerDeliveryParameter_isUsed) {
                outJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVReady"] = body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVReady;
                outJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVErrorCode"] = body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVErrorCode;
                outJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVRESSSOC"] = body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVRESSSOC;

                if (body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.BulkChargingComplete_isUsed) {
                    outJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["BulkChargingComplete"] = body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.BulkChargingComplete;
                }

                outJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["ChargingComplete"] = body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.ChargingComplete;
            }

            if (body.PowerDeliveryReq.EVPowerDeliveryParameter_isUsed) {
                outJson["Body"]["PowerDeliveryReq"]["EVPowerDeliveryParameter"]["_unused"] = body.PowerDeliveryReq.EVPowerDeliveryParameter._unused;
            }

        } else if (body.PowerDeliveryRes_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["PowerDeliveryRes"]["ResponseCode"] = body.PowerDeliveryRes.ResponseCode;

            if (body.PowerDeliveryRes.AC_EVSEStatus_isUsed) {
                outJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["NotificationMaxDelay"] = body.PowerDeliveryRes.AC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["EVSENotification"] = body.PowerDeliveryRes.AC_EVSEStatus.EVSENotification;
                outJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["RCD"] = body.PowerDeliveryRes.AC_EVSEStatus.RCD;
            }

            if (body.PowerDeliveryRes.DC_EVSEStatus_isUsed) {
                outJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.PowerDeliveryRes.DC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["EVSENotification"] = body.PowerDeliveryRes.DC_EVSEStatus.EVSENotification;
                if (body.PowerDeliveryRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed) {
                    outJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.PowerDeliveryRes.DC_EVSEStatus.EVSEIsolationStatus;
                }
                outJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.PowerDeliveryRes.DC_EVSEStatus.EVSEStatusCode;
            }

            if (body.PowerDeliveryRes.EVSEStatus_isUsed) {
                outJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["NotificationMaxDelay"] = body.PowerDeliveryRes.EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["EVSENotification"] = body.PowerDeliveryRes.EVSEStatus.EVSENotification;

                outJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["AC_EVSEStatus"]["NotificationMaxDelay"] = body.PowerDeliveryRes.EVSEStatus.AC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["AC_EVSEStatus"]["EVSENotification"] = body.PowerDeliveryRes.EVSEStatus.AC_EVSEStatus.EVSENotification;
                outJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["AC_EVSEStatus"]["RCD"] = body.PowerDeliveryRes.EVSEStatus.AC_EVSEStatus.RCD;

                outJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.PowerDeliveryRes.EVSEStatus.DC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["DC_EVSEStatus"]["EVSENotification"] = body.PowerDeliveryRes.EVSEStatus.DC_EVSEStatus.EVSENotification;
                if (body.PowerDeliveryRes.EVSEStatus.DC_EVSEStatus.EVSEIsolationStatus_isUsed) {
                    outJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.PowerDeliveryRes.EVSEStatus.DC_EVSEStatus.EVSEIsolationStatus;
                }
                outJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.PowerDeliveryRes.EVSEStatus.DC_EVSEStatus.EVSEStatusCode;
            }

        } else if (body.PreChargeReq_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVReady"] = body.PreChargeReq.DC_EVStatus.EVReady;
            outJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVErrorCode"] = body.PreChargeReq.DC_EVStatus.EVErrorCode;
            outJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVRESSSOC"] = body.PreChargeReq.DC_EVStatus.EVRESSSOC;

            outJson["Body"]["PreChargeReq"]["EVTargetVoltage"]["Multiplier"] = body.PreChargeReq.EVTargetVoltage.Multiplier;
            outJson["Body"]["PreChargeReq"]["EVTargetVoltage"]["Unit"] = body.PreChargeReq.EVTargetVoltage.Unit;
            outJson["Body"]["PreChargeReq"]["EVTargetVoltage"]["Value"] = body.PreChargeReq.EVTargetVoltage.Value;

            outJson["Body"]["PreChargeReq"]["EVTargetCurrent"]["Multiplier"] = body.PreChargeReq.EVTargetCurrent.Multiplier;
            outJson["Body"]["PreChargeReq"]["EVTargetCurrent"]["Unit"] = body.PreChargeReq.EVTargetCurrent.Unit;
            outJson["Body"]["PreChargeReq"]["EVTargetCurrent"]["Value"] = body.PreChargeReq.EVTargetCurrent.Value;

        } else if (body.PreChargeRes_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["PreChargeRes"]["ResponseCode"] = body.PreChargeRes.ResponseCode;

            outJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.PreChargeRes.DC_EVSEStatus.NotificationMaxDelay;
            outJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["EVSENotification"] = body.PreChargeRes.DC_EVSEStatus.EVSENotification;
            if (body.PreChargeRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed) {
                outJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.PreChargeRes.DC_EVSEStatus.EVSEIsolationStatus;
            }
            outJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.PreChargeRes.DC_EVSEStatus.EVSEStatusCode;

            outJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"]["Multiplier"] = body.PreChargeRes.EVSEPresentVoltage.Multiplier;
            outJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"]["Unit"] = body.PreChargeRes.EVSEPresentVoltage.Unit;
            outJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"]["Value"] = body.PreChargeRes.EVSEPresentVoltage.Value;

        } else if (body.ServiceDetailReq_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["ServiceDetailReq"]["ServiceID"] = body.ServiceDetailReq.ServiceID;

        } else if (body.ServiceDetailRes_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["ServiceDetailRes"]["ResponseCode"] = body.ServiceDetailRes.ResponseCode;

            outJson["Body"]["ServiceDetailRes"]["ServiceID"] = body.ServiceDetailRes.ServiceID;
            
            if (body.ServiceDetailRes.ServiceParameterList_isUsed) {    
                outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["arrayLen"] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.arrayLen;
                for (int i = 0; i < body.ServiceDetailRes.ServiceParameterList.ParameterSet.arrayLen; i++) {
                    outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["ParameterSetID"] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].ParameterSetID;
                    outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["arrayLen"] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.arrayLen;
                    for (int j = 0; j < body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.arrayLen; j++) {
                        outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["Name"]["charactersLen"] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].Name.charactersLen;
                        for (int k = 0; k < body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].Name.charactersLen; k++) {
                            outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["Name"]["characters"][k] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].Name.characters[k];
                        }
                        if (body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].boolValue_isUsed) {
                            outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["boolValue"] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].boolValue;
                        }
                        if (body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].byteValue_isUsed) {
                            outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["byteValue"] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].byteValue;
                        }
                        if (body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].shortValue_isUsed) {
                            outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["shortValue"] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].shortValue;
                        }
                        if (body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].intValue_isUsed) {
                            outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["intValue"] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].intValue;
                        }
                        if (body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].physicalValue_isUsed) {
                            outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["physicalValue"]["Multiplier"] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].physicalValue.Multiplier;
                            outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["physicalValue"]["Unit"] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].physicalValue.Unit;
                            outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["physicalValue"]["Value"] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].physicalValue.Value;
                        }
                        if (body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].stringValue_isUsed) {
                            outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["stringValue"]["charactersLen"] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].stringValue.charactersLen;
                            for (int k = 0; k < body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].stringValue.charactersLen; k++) {
                                outJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["stringValue"]["characters"][k] = body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].stringValue.characters[k];
                            }
                        }
                    }
                }
            }

        } else if (body.ServiceDiscoveryReq_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            if (body.ServiceDiscoveryReq.ServiceScope_isUsed) {
                outJson["Body"]["ServiceDiscoveryReq"]["ServiceScope"]["charactersLen"] = body.ServiceDiscoveryReq.ServiceScope.charactersLen;
                for (int i = 0; i < body.ServiceDiscoveryReq.ServiceScope.charactersLen; i++) {
                    outJson["Body"]["ServiceDiscoveryReq"]["ServiceScope"]["characters"][i] = body.ServiceDiscoveryReq.ServiceScope.characters[i];
                }
            }

            if (body.ServiceDiscoveryReq.ServiceCategory_isUsed) {
                outJson["Body"]["ServiceDiscoveryReq"]["ServiceCategory"] = body.ServiceDiscoveryReq.ServiceCategory;
            }

            if (!(body.ServiceDiscoveryReq.ServiceScope_isUsed | body.ServiceDiscoveryReq.ServiceCategory_isUsed)) {
                outJson["Body"]["ServiceDiscoveryReq"]["isUsed"] = true;
            }

        } else if (body.ServiceDiscoveryRes_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["ServiceDiscoveryRes"]["ResponseCode"] = body.ServiceDiscoveryRes.ResponseCode;

            outJson["Body"]["ServiceDiscoveryRes"]["PaymentOptionList"]["PaymentOption"]["arrayLen"] = body.ServiceDiscoveryRes.PaymentOptionList.PaymentOption.arrayLen;
            for (int i = 0; i < body.ServiceDiscoveryRes.PaymentOptionList.PaymentOption.arrayLen; i++) {
                outJson["Body"]["ServiceDiscoveryRes"]["PaymentOptionList"]["PaymentOption"]["array"][i] = body.ServiceDiscoveryRes.PaymentOptionList.PaymentOption.array[i];
            }

            outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceID"] = body.ServiceDiscoveryRes.ChargeService.ServiceID;

            if (body.ServiceDiscoveryRes.ChargeService.ServiceName_isUsed) {
                outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceName"]["charactersLen"] = body.ServiceDiscoveryRes.ChargeService.ServiceName.charactersLen;
                for (int i = 0; i < body.ServiceDiscoveryRes.ChargeService.ServiceName.charactersLen; i++) {
                    outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceName"]["characters"][i] = body.ServiceDiscoveryRes.ChargeService.ServiceName.characters[i];
                }
            }

            outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceCategory"] = body.ServiceDiscoveryRes.ChargeService.ServiceCategory;

            if (body.ServiceDiscoveryRes.ChargeService.ServiceScope_isUsed) {
                outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceScope"]["charactersLen"] = body.ServiceDiscoveryRes.ChargeService.ServiceScope.charactersLen;
                for (int i = 0; i < body.ServiceDiscoveryRes.ChargeService.ServiceScope.charactersLen; i++) {
                    outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceScope"]["characters"][i] = body.ServiceDiscoveryRes.ChargeService.ServiceScope.characters[i];
                }
            }

            outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["FreeService"] = body.ServiceDiscoveryRes.ChargeService.FreeService;

            outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["SupportedEnergyTransferMode"]["EnergyTransferMode"]["arrayLen"] = body.ServiceDiscoveryRes.ChargeService.SupportedEnergyTransferMode.EnergyTransferMode.arrayLen;
            for (int i = 0; i < body.ServiceDiscoveryRes.ChargeService.SupportedEnergyTransferMode.EnergyTransferMode.arrayLen; i++) {
                outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["SupportedEnergyTransferMode"]["EnergyTransferMode"]["array"][i] = body.ServiceDiscoveryRes.ChargeService.SupportedEnergyTransferMode.EnergyTransferMode.array[i];
            }

            if (body.ServiceDiscoveryRes.ServiceList_isUsed) {
                outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["arrayLen"] = body.ServiceDiscoveryRes.ServiceList.Service.arrayLen;
                for (int i = 0; i < body.ServiceDiscoveryRes.ServiceList.Service.arrayLen; i++) {
                    outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["ServiceID"] = body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceID;
                    if (body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceName_isUsed) {
                        outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["ServiceName"]["charactersLen"] = body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceName.charactersLen;
                        for (int j = 0; j < body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceName.charactersLen; j++) {
                            outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["ServiceName"]["characters"][j] = body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceName.characters[j];
                        }
                    }
                    outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["ServiceCategory"] = body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceCategory;
                    if (body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceScope_isUsed) {
                        outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["ServiceScope"]["charactersLen"] = body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceScope.charactersLen;
                        for (int j = 0; j < body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceScope.charactersLen; j++) {
                            outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["ServiceScope"]["characters"][j] = body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceScope.characters[j];
                        }
                    }
                    outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["FreeService"] = body.ServiceDiscoveryRes.ServiceList.Service.array[i].FreeService;
                }
            }

        } else if (body.SessionSetupReq_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["SessionSetupReq"]["EVCCID"]["bytesLen"] = body.SessionSetupReq.EVCCID.bytesLen;
            for (int i = 0; i < body.SessionSetupReq.EVCCID.bytesLen; i++) {
                outJson["Body"]["SessionSetupReq"]["EVCCID"]["bytes"][i] = body.SessionSetupReq.EVCCID.bytes[i];
            }

        } else if (body.SessionSetupRes_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["SessionSetupRes"]["ResponseCode"] = body.SessionSetupRes.ResponseCode;

            outJson["Body"]["SessionSetupRes"]["EVSEID"]["charactersLen"] = body.SessionSetupRes.EVSEID.charactersLen;
            for (int i = 0; i < body.SessionSetupRes.EVSEID.charactersLen; i++) {
                outJson["Body"]["SessionSetupRes"]["EVSEID"]["characters"][i] = body.SessionSetupRes.EVSEID.characters[i];
            }

            if (body.SessionSetupRes.EVSETimeStamp_isUsed) {
                outJson["Body"]["SessionSetupRes"]["EVSETimeStamp"] = body.SessionSetupRes.EVSETimeStamp;
            }

        } else if (body.SessionStopReq_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["SessionStopReq"]["ChargingSession"] = body.SessionStopReq.ChargingSession;

        } else if (body.SessionStopRes_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["SessionStopRes"]["ResponseCode"] = body.SessionStopRes.ResponseCode;

        } else if (body.WeldingDetectionReq_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["WeldingDetectionReq"]["DC_EVStatus"]["EVReady"] = body.WeldingDetectionReq.DC_EVStatus.EVReady;
            outJson["Body"]["WeldingDetectionReq"]["DC_EVStatus"]["EVErrorCode"] = body.WeldingDetectionReq.DC_EVStatus.EVErrorCode;
            outJson["Body"]["WeldingDetectionReq"]["DC_EVStatus"]["EVRESSSOC"] = body.WeldingDetectionReq.DC_EVStatus.EVRESSSOC;

        } else if (body.WeldingDetectionRes_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["WeldingDetectionRes"]["ResponseCode"] = body.WeldingDetectionRes.ResponseCode;

            outJson["Body"]["WeldingDetectionRes"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.WeldingDetectionRes.DC_EVSEStatus.NotificationMaxDelay;
            outJson["Body"]["WeldingDetectionRes"]["DC_EVSEStatus"]["EVSENotification"] = body.WeldingDetectionRes.DC_EVSEStatus.EVSENotification;
            if (body.WeldingDetectionRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed) {
                outJson["Body"]["WeldingDetectionRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.WeldingDetectionRes.DC_EVSEStatus.EVSEIsolationStatus;
            }
            outJson["Body"]["WeldingDetectionRes"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.WeldingDetectionRes.DC_EVSEStatus.EVSEStatusCode;

            outJson["Body"]["WeldingDetectionRes"]["EVSEPresentVoltage"]["Multiplier"] = body.WeldingDetectionRes.EVSEPresentVoltage.Multiplier;
            outJson["Body"]["WeldingDetectionRes"]["EVSEPresentVoltage"]["Unit"] = body.WeldingDetectionRes.EVSEPresentVoltage.Unit;
            outJson["Body"]["WeldingDetectionRes"]["EVSEPresentVoltage"]["Value"] = body.WeldingDetectionRes.EVSEPresentVoltage.Value;

        } else {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        }

        static string jsonString;
        jsonString = outJson.dump(4);

        return jsonString.c_str();
    }

    struct encoded_data {
        uint8_t* buffer;
        size_t size;
    };

    encoded_data* encodeFromPythonDict(const char* inString) {
        return nullptr;
    }

    void free_encoded_data(encoded_data* data) {
        if (data) {
            delete[] data->buffer;
            delete data;
        }
    }
}

int main(int argc, char* argv[]) {
    cout << "ISO2Processor is not yet implemented." << endl;
    return 0;
}