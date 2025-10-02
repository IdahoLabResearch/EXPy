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
            outJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["EVSENotification"] = body.CableCheckRes.DC_EVSEStatus.EVSENotification;
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

            if (body.CertificateUpdateReq.ContractSignatureCertChain.Id_isUsed) {
                outJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["Id"]["charactersLen"] = body.CertificateUpdateReq.ContractSignatureCertChain.Id.charactersLen;
                for (int i = 0; i < body.CertificateUpdateReq.ContractSignatureCertChain.Id.charactersLen; i++) {
                    outJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["Id"]["characters"][i] = body.CertificateUpdateReq.ContractSignatureCertChain.Id.characters[i];
                }
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
                            outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["TimeInterval"]["_unused"] = 0;
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
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["TimeInterval"]["_unused"] = 0;
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
                outJson["Body"]["ChargeParameterDiscoveryRes"]["SASchedules"]["_unused"] = 0;
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
                outJson["Body"]["ChargeParameterDiscoveryRes"]["EVSEChargeParameter"]["_unused"] = 0;
            }
        } else if (body.ChargingStatusReq_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            outJson["Body"]["ChargingStatusReq"]["_unused"] = 0;

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
            outJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"]["Unit"] = body.CurrentDemandReq.EVTargetVoltage.Unit;
            outJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"]["Value"] = body.CurrentDemandReq.EVTargetVoltage.Value;

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
                outJson["Body"]["PowerDeliveryReq"]["EVPowerDeliveryParameter"]["_unused"] = 0;
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
            
            // Cant make empty json object, can I? idk, im pretty sure I cant
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
        json inJson = json::parse(inString);
        auto sessionID = inJson["Header"]["SessionID"]["bytes"].template get<std::array<uint8_t, 8>>();

        iso2_exiDocument inDoc;
        init_iso2_exiDocument(&inDoc);
        init_iso2_MessageHeaderType(&inDoc.V2G_Message.Header);

        auto& header = inDoc.V2G_Message.Header;
        header.SessionID.bytesLen = iso2_sessionIDType_BYTES_SIZE;
        copy(sessionID.begin(), sessionID.end(), header.SessionID.bytes);

        init_iso2_BodyType(&inDoc.V2G_Message.Body);
        auto& body = inDoc.V2G_Message.Body;

        string packetType = inJson["Body"].items().begin().key();

        if (packetType == "AuthorizationReq") {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_AuthorizationReqType(&body.AuthorizationReq);
            body.AuthorizationReq_isUsed = true;

            if (inJson["Body"]["AuthorizationReq"].contains("Id")) {
                body.AuthorizationReq.Id_isUsed = true;
                body.AuthorizationReq.Id.charactersLen = inJson["Body"]["AuthorizationReq"]["Id"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.AuthorizationReq.Id.charactersLen; i++) {
                    body.AuthorizationReq.Id.characters[i] = inJson["Body"]["Authorization"]["Id"]["characters"][i].template get<char>();
                }
            } else {
                body.AuthorizationReq.Id_isUsed = false;
            }

            if (inJson["Body"]["AuthorizationReq"].contains("GenChallenge")) {
                body.AuthorizationReq.GenChallenge_isUsed = true;
                body.AuthorizationReq.GenChallenge.bytesLen = inJson["Body"]["AuthorizationReq"]["GenChallenge"]["bytesLen"].template get<uint16_t>();
                for (int i = 0; i < body.AuthorizationReq.GenChallenge.bytesLen; i++) {
                    body.AuthorizationReq.GenChallenge.bytes[i] = inJson["Body"]["AuthorizationReq"]["GenChallenge"]["bytes"][i].template get<uint8_t>();
                }
            } else {
                body.AuthorizationReq.GenChallenge_isUsed = false;
            }
        } else if (packetType == "AuthorizationRes") {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_AuthorizationResType(&body.AuthorizationRes);
            body.AuthorizationRes_isUsed = true;

            body.AuthorizationRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["AuthorizationRes"]["ResponseCode"].template get<int>());
            body.AuthorizationRes.EVSEProcessing = static_cast<iso2_EVSEProcessingType>(inJson["Body"]["AuthorizationRes"]["EVSEProcessing"].template get<int>());
        } else if (packetType == "CableCheckReq") {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_CableCheckReqType(&body.CableCheckReq);
            body.CableCheckReq_isUsed = true;

            body.CableCheckReq.DC_EVStatus.EVReady = inJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVReady"].template get<int>();
            body.CableCheckReq.DC_EVStatus.EVErrorCode = static_cast<iso2_DC_EVErrorCodeType>(inJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVErrorCode"].template get<int>());
            body.CableCheckReq.DC_EVStatus.EVRESSSOC = inJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVRESSSOC"].template get<uint8_t>();
        } else if (packetType == "CableCheckRes") {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_CableCheckResType(&body.CableCheckRes);
            body.CableCheckRes_isUsed = true;

            body.CableCheckRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["CableCheckRes"]["ResponseCode"].template get<int>());

            body.CableCheckRes.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
            body.CableCheckRes.DC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());
            if (inJson["Body"]["CableCheckRes"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                body.CableCheckRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                body.CableCheckRes.DC_EVSEStatus.EVSEIsolationStatus = static_cast<iso2_isolationLevelType>(inJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
            } else {
                body.CableCheckRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
            }
            body.CableCheckRes.EVSEProcessing = static_cast<iso2_EVSEProcessingType>(inJson["Body"]["CableCheckRes"]["EVSEProcessing"].template get<int>());

        } else if (packetType == "CertificateInstallationReq") {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_CertificateInstallationReqType(&body.CertificateInstallationReq);
            body.CertificateInstallationReq_isUsed = true;

            body.CertificateInstallationReq.Id.charactersLen = inJson["Body"]["CertificateInstallationReq"]["Id"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateInstallationReq.Id.charactersLen; i++) {
                body.CertificateInstallationReq.Id.characters[i] = inJson["Body"]["CertificateInstallationReq"]["Id"]["characters"][i].template get<char>();
            }

            body.CertificateInstallationReq.OEMProvisioningCert.bytesLen = inJson["Body"]["CertificateInstallationReq"]["OEMProvisioningCert"]["bytesLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateInstallationReq.OEMProvisioningCert.bytesLen; i++) {
                body.CertificateInstallationReq.OEMProvisioningCert.bytes[i] = inJson["Body"]["CertificateInstallationReq"]["OEMProvisioningCert"]["bytes"][i].template get<uint8_t>();
            }

            body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.arrayLen = inJson["Body"]["CertificateInstallationReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["arrayLen"].template get<uint8_t>();
            for (int i = 0; i < body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.arrayLen; i++) {
                body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509IssuerName.charactersLen = inJson["Body"]["CertificateInstallationReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509IssuerName"]["charactersLen"].template get<uint16_t>();
                for (int j = 0; j < body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509IssuerName.charactersLen; j++) {
                    body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509IssuerName.characters[j] = inJson["Body"]["CertificateInstallationReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509IssuerName"]["characters"][j].template get<char>();
                }
                body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.data.octets_count = inJson["Body"]["CertificateInstallationReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509SerialNumber"]["data"]["octetsCount"].template get<uint8_t>();
                for (size_t j = 0; j < body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.data.octets_count; j++) {
                    body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.data.octets[j] = inJson["Body"]["CertificateInstallationReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509SerialNumber"]["data"]["octets"][j].template get<uint8_t>();
                }
                body.CertificateInstallationReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.is_negative = inJson["Body"]["CertificateInstallationReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509SerialNumber"]["isNegative"].template get<bool>();
            }
        } else if (packetType == "CertificateInstallationRes") {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_CertificateInstallationResType(&body.CertificateInstallationRes);
            body.CertificateInstallationRes_isUsed = true;

            body.CertificateInstallationRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["CertificateInstallationRes"]["ResponseCode"].template get<int>());

            if (inJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"].contains("Id")) {
                body.CertificateInstallationRes.SAProvisioningCertificateChain.Id_isUsed = true;
                body.CertificateInstallationRes.SAProvisioningCertificateChain.Id.charactersLen = inJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["Id"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.CertificateInstallationRes.SAProvisioningCertificateChain.Id.charactersLen; i++) {
                    body.CertificateInstallationRes.SAProvisioningCertificateChain.Id.characters[i] = inJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["Id"]["characters"][i].template get<char>();
                }
            } else {
                body.CertificateInstallationRes.SAProvisioningCertificateChain.Id_isUsed = false;
            }

            body.CertificateInstallationRes.SAProvisioningCertificateChain.Certificate.bytesLen = inJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["Certificate"]["bytesLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateInstallationRes.SAProvisioningCertificateChain.Certificate.bytesLen; i++) {
                body.CertificateInstallationRes.SAProvisioningCertificateChain.Certificate.bytes[i] = inJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["Certificate"]["bytes"][i].template get<uint8_t>();
            }

            if (inJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"].contains("SubCertificates")) {
                body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates_isUsed = true;
                body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates.Certificate.arrayLen = inJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["SubCertificates"]["Certificate"]["arrayLen"].template get<uint8_t>();
                for (int i = 0; i < body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates.Certificate.arrayLen; i++) {
                    body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates.Certificate.array[i].bytesLen = inJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["SubCertificates"]["Certificate"]["array"][i]["bytesLen"].template get<uint16_t>();
                    for (int j = 0; j < body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates.Certificate.array[i].bytesLen; j++) {
                        body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates.Certificate.array[i].bytes[j] = inJson["Body"]["CertificateInstallationRes"]["SAProvisioningCertificateChain"]["SubCertificates"]["Certificate"]["array"][i]["bytes"][j].template get<uint8_t>();
                    }
                }
            } else {
                body.CertificateInstallationRes.SAProvisioningCertificateChain.SubCertificates_isUsed = false;
            }
            
            if (inJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"].contains("Id")) {
                body.CertificateInstallationRes.ContractSignatureCertChain.Id_isUsed = true;
                body.CertificateInstallationRes.ContractSignatureCertChain.Id.charactersLen = inJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["Id"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.CertificateInstallationRes.ContractSignatureCertChain.Id.charactersLen; i++) {
                    body.CertificateInstallationRes.ContractSignatureCertChain.Id.characters[i] = inJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["Id"]["characters"][i].template get<char>();
                }
            } else {
                body.CertificateInstallationRes.ContractSignatureCertChain.Id_isUsed = false;
            }
            body.CertificateInstallationRes.ContractSignatureCertChain.Certificate.bytesLen = inJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["Certificate"]["bytesLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateInstallationRes.ContractSignatureCertChain.Certificate.bytesLen; i++) {
                body.CertificateInstallationRes.ContractSignatureCertChain.Certificate.bytes[i] = inJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["Certificate"]["bytes"][i].template get<uint8_t>();
            }
            if (inJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"].contains("SubCertificates")) {
                body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates_isUsed = true;
                body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen = inJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["arrayLen"].template get<uint8_t>();
                for (int i = 0; i < body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen; i++) {
                    body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen = inJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytesLen"].template get<uint16_t>();
                    for (int j = 0; j < body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen; j++) {
                        body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytes[j] = inJson["Body"]["CertificateInstallationRes"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytes"][j].template get<uint8_t>();
                    }
                }
            } else {
                body.CertificateInstallationRes.ContractSignatureCertChain.SubCertificates_isUsed = false;
            }

            body.CertificateInstallationRes.ContractSignatureEncryptedPrivateKey.Id.charactersLen = inJson["Body"]["CertificateInstallationRes"]["ContractSignatureEncryptedPrivateKey"]["Id"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateInstallationRes.ContractSignatureEncryptedPrivateKey.Id.charactersLen; i++) {
                body.CertificateInstallationRes.ContractSignatureEncryptedPrivateKey.Id.characters[i] = inJson["Body"]["CertificateInstallationRes"]["ContractSignatureEncryptedPrivateKey"]["Id"]["characters"][i].template get<char>();
            }

            body.CertificateInstallationRes.ContractSignatureEncryptedPrivateKey.CONTENT.bytesLen = inJson["Body"]["CertificateInstallationRes"]["ContractSignatureEncryptedPrivateKey"]["CONTENT"]["bytesLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateInstallationRes.ContractSignatureEncryptedPrivateKey.CONTENT.bytesLen; i++) {
                body.CertificateInstallationRes.ContractSignatureEncryptedPrivateKey.CONTENT.bytes[i] = inJson["Body"]["CertificateInstallationRes"]["ContractSignatureEncryptedPrivateKey"]["CONTENT"]["bytes"][i].template get<uint8_t>();
            }

            body.CertificateInstallationRes.DHpublickey.Id.charactersLen = inJson["Body"]["CertificateInstallationRes"]["DHpublickey"]["Id"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateInstallationRes.DHpublickey.Id.charactersLen; i++) {
                body.CertificateInstallationRes.DHpublickey.Id.characters[i] = inJson["Body"]["CertificateInstallationRes"]["DHpublickey"]["Id"]["characters"][i].template get<char>();
            }

            body.CertificateInstallationRes.DHpublickey.CONTENT.bytesLen = inJson["Body"]["CertificateInstallationRes"]["DHpublickey"]["CONTENT"]["bytesLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateInstallationRes.DHpublickey.CONTENT.bytesLen; i++) {
                body.CertificateInstallationRes.DHpublickey.CONTENT.bytes[i] = inJson["Body"]["CertificateInstallationRes"]["DHpublickey"]["CONTENT"]["bytes"][i].template get<uint8_t>();
            }

            body.CertificateUpdateRes.eMAID.Id.charactersLen = inJson["Body"]["CertificateUpdateRes"]["eMAID"]["Id"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateRes.eMAID.Id.charactersLen; i++) {
                body.CertificateUpdateRes.eMAID.Id.characters[i] = inJson["Body"]["CertificateUpdateRes"]["eMAID"]["Id"]["characters"][i].template get<char>();
            }

            body.CertificateUpdateRes.eMAID.CONTENT.charactersLen = inJson["Body"]["CertificateUpdateRes"]["eMAID"]["CONTENT"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateRes.eMAID.CONTENT.charactersLen; i++) {
                body.CertificateUpdateRes.eMAID.CONTENT.characters[i] = inJson["Body"]["CertificateUpdateRes"]["eMAID"]["CONTENT"]["characters"][i].template get<char>();
            }

        } else if (packetType == "CertificateUpdateReq") {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_CertificateUpdateReqType(&body.CertificateUpdateReq);
            body.CertificateUpdateReq_isUsed = true;

            body.CertificateUpdateReq.Id.charactersLen = inJson["Body"]["CertificateUpdateReq"]["Id"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateReq.Id.charactersLen; i++) {
                body.CertificateUpdateReq.Id.characters[i] = inJson["Body"]["CertificateUpdateReq"]["Id"]["characters"][i].template get<char>();
            }

            if (inJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"].contains("Id")) {
                body.CertificateUpdateReq.ContractSignatureCertChain.Id_isUsed = true;
                body.CertificateUpdateReq.ContractSignatureCertChain.Id.charactersLen = inJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["Id"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.CertificateUpdateReq.ContractSignatureCertChain.Id.charactersLen; i++) {
                    body.CertificateUpdateReq.ContractSignatureCertChain.Id.characters[i] = inJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["Id"]["characters"][i].template get<char>();
                }
            } else {
                body.CertificateUpdateReq.ContractSignatureCertChain.Id_isUsed = false;
            }
            body.CertificateUpdateReq.ContractSignatureCertChain.Certificate.bytesLen = inJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["Certificate"]["bytesLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateReq.ContractSignatureCertChain.Certificate.bytesLen; i++) {
                body.CertificateUpdateReq.ContractSignatureCertChain.Certificate.bytes[i] = inJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["Certificate"]["bytes"][i].template get<uint8_t>();
            }
            if (inJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"].contains("SubCertificates")) {
                body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates_isUsed = true;
                body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen = inJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["arrayLen"].template get<uint8_t>();
                for (int i = 0; i < body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen; i++) {
                    body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen = inJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytesLen"].template get<uint16_t>();
                    for (int j = 0; j < body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen; j++) {
                        body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytes[j] = inJson["Body"]["CertificateUpdateReq"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytes"][j].template get<uint8_t>();
                    }
                }
            } else {
                body.CertificateUpdateReq.ContractSignatureCertChain.SubCertificates_isUsed = false;
            }

            body.CertificateUpdateReq.eMAID.charactersLen = inJson["Body"]["CertificateUpdateReq"]["eMAID"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateReq.eMAID.charactersLen; i++) {
                body.CertificateUpdateReq.eMAID.characters[i] = inJson["Body"]["CertificateUpdateReq"]["eMAID"]["characters"][i].template get<char>();
            }

            body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.arrayLen = inJson["Body"]["CertificateUpdateReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["arrayLen"].template get<uint8_t>();
            for (int i = 0; i < body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.arrayLen; i++) {
                body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509IssuerName.charactersLen = inJson["Body"]["CertificateUpdateReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509IssuerName"]["charactersLen"].template get<uint16_t>();
                for (int j = 0; j < body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509IssuerName.charactersLen; j++) {
                    body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509IssuerName.characters[j] = inJson["Body"]["CertificateUpdateReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509IssuerName"]["characters"][j].template get<char>();
                }
                body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.data.octets_count = inJson["Body"]["CertificateUpdateReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509SerialNumber"]["data"]["octetsCount"].template get<uint8_t>();
                for (size_t j = 0; j < body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.data.octets_count; j++) {
                    body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.data.octets[j] = inJson["Body"]["CertificateUpdateReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509SerialNumber"]["data"]["octets"][j].template get<uint8_t>();
                }
                body.CertificateUpdateReq.ListOfRootCertificateIDs.RootCertificateID.array[i].X509SerialNumber.is_negative = inJson["Body"]["CertificateUpdateReq"]["ListOfRootCertificateIDs"]["RootCertificateID"]["array"][i]["X509SerialNumber"]["isNegative"].template get<bool>();
            }
        
        } else if (packetType == "CertificateUpdateRes") {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_CertificateUpdateResType(&body.CertificateUpdateRes);
            body.CertificateUpdateRes_isUsed = true;

            body.CertificateUpdateRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["CertificateUpdateRes"]["ResponseCode"].template get<int>());

            if (inJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"].contains("Id")) {
                body.CertificateUpdateRes.SAProvisioningCertificateChain.Id_isUsed = true;
                body.CertificateUpdateRes.SAProvisioningCertificateChain.Id.charactersLen = inJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["Id"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.CertificateUpdateRes.SAProvisioningCertificateChain.Id.charactersLen; i++) {
                    body.CertificateUpdateRes.SAProvisioningCertificateChain.Id.characters[i] = inJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["Id"]["characters"][i].template get<char>();
                }
            } else {
                body.CertificateUpdateRes.SAProvisioningCertificateChain.Id_isUsed = false;
            }

            body.CertificateUpdateRes.SAProvisioningCertificateChain.Certificate.bytesLen = inJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["Certificate"]["bytesLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateRes.SAProvisioningCertificateChain.Certificate.bytesLen; i++) {
                body.CertificateUpdateRes.SAProvisioningCertificateChain.Certificate.bytes[i] = inJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["Certificate"]["bytes"][i].template get<uint8_t>();
            }

            if (inJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"].contains("SubCertificates")) {
                body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates_isUsed = true;
                body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates.Certificate.arrayLen = inJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["SubCertificates"]["Certificate"]["arrayLen"].template get<uint8_t>();
                for (int i = 0; i < body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates.Certificate.arrayLen; i++) {
                    body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates.Certificate.array[i].bytesLen = inJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["SubCertificates"]["Certificate"]["array"][i]["bytesLen"].template get<uint16_t>();
                    for (int j = 0; j < body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates.Certificate.array[i].bytesLen; j++) {
                        body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates.Certificate.array[i].bytes[j] = inJson["Body"]["CertificateUpdateRes"]["SAProvisioningCertificateChain"]["SubCertificates"]["Certificate"]["array"][i]["bytes"][j].template get<uint8_t>();
                    }
                }
            } else {
                body.CertificateUpdateRes.SAProvisioningCertificateChain.SubCertificates_isUsed = false;
            }

            if (inJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"].contains("Id")) {
                body.CertificateUpdateRes.ContractSignatureCertChain.Id_isUsed = true;
                body.CertificateUpdateRes.ContractSignatureCertChain.Id.charactersLen = inJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["Id"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.CertificateUpdateRes.ContractSignatureCertChain.Id.charactersLen; i++) {
                    body.CertificateUpdateRes.ContractSignatureCertChain.Id.characters[i] = inJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["Id"]["characters"][i].template get<char>();
                }
            } else {
                body.CertificateUpdateRes.ContractSignatureCertChain.Id_isUsed = false;
            }
            body.CertificateUpdateRes.ContractSignatureCertChain.Certificate.bytesLen = inJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["Certificate"]["bytesLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateRes.ContractSignatureCertChain.Certificate.bytesLen; i++) {
                body.CertificateUpdateRes.ContractSignatureCertChain.Certificate.bytes[i] = inJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["Certificate"]["bytes"][i].template get<uint8_t>();
            }
            if (inJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"].contains("SubCertificates")) {
                body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates_isUsed = true;
                body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen = inJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["arrayLen"].template get<uint8_t>();
                for (int i = 0; i < body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen; i++) {
                    body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen = inJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytesLen"].template get<uint16_t>();
                    for (int j = 0; j < body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen; j++) {
                        body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytes[j] = inJson["Body"]["CertificateUpdateRes"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytes"][j].template get<uint8_t>();
                    }
                }
            } else {
                body.CertificateUpdateRes.ContractSignatureCertChain.SubCertificates_isUsed = false;
            }

            body.CertificateUpdateRes.ContractSignatureEncryptedPrivateKey.Id.charactersLen = inJson["Body"]["CertificateUpdateRes"]["ContractSignatureEncryptedPrivateKey"]["Id"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateRes.ContractSignatureEncryptedPrivateKey.Id.charactersLen; i++) {
                body.CertificateUpdateRes.ContractSignatureEncryptedPrivateKey.Id.characters[i] = inJson["Body"]["CertificateUpdateRes"]["ContractSignatureEncryptedPrivateKey"]["Id"]["characters"][i].template get<char>();
            }

            body.CertificateUpdateRes.ContractSignatureEncryptedPrivateKey.CONTENT.bytesLen = inJson["Body"]["CertificateUpdateRes"]["ContractSignatureEncryptedPrivateKey"]["CONTENT"]["bytesLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateRes.ContractSignatureEncryptedPrivateKey.CONTENT.bytesLen; i++) {
                body.CertificateUpdateRes.ContractSignatureEncryptedPrivateKey.CONTENT.bytes[i] = inJson["Body"]["CertificateUpdateRes"]["ContractSignatureEncryptedPrivateKey"]["CONTENT"]["bytes"][i].template get<uint8_t>();
            }

            body.CertificateUpdateRes.DHpublickey.Id.charactersLen = inJson["Body"]["CertificateUpdateRes"]["DHpublickey"]["Id"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateRes.DHpublickey.Id.charactersLen; i++) {
                body.CertificateUpdateRes.DHpublickey.Id.characters[i] = inJson["Body"]["CertificateUpdateRes"]["DHpublickey"]["Id"]["characters"][i].template get<char>();
            }
            body.CertificateUpdateRes.DHpublickey.CONTENT.bytesLen = inJson["Body"]["CertificateUpdateRes"]["DHpublickey"]["CONTENT"]["bytesLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateRes.DHpublickey.CONTENT.bytesLen; i++) {
                body.CertificateUpdateRes.DHpublickey.CONTENT.bytes[i] = inJson["Body"]["CertificateUpdateRes"]["DHpublickey"]["CONTENT"]["bytes"][i].template get<uint8_t>();
            }

            body.CertificateUpdateRes.eMAID.Id.charactersLen = inJson["Body"]["CertificateUpdateRes"]["eMAID"]["Id"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateRes.eMAID.Id.charactersLen; i++) {
                body.CertificateUpdateRes.eMAID.Id.characters[i] = inJson["Body"]["CertificateUpdateRes"]["eMAID"]["Id"]["characters"][i].template get<char>();
            }
            body.CertificateUpdateRes.eMAID.CONTENT.charactersLen = inJson["Body"]["CertificateUpdateRes"]["eMAID"]["CONTENT"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.CertificateUpdateRes.eMAID.CONTENT.charactersLen; i++) {
                body.CertificateUpdateRes.eMAID.CONTENT.characters[i] = inJson["Body"]["CertificateUpdateRes"]["eMAID"]["CONTENT"]["characters"][i].template get<char>();
            }
        } else if (packetType == "ChargeParameterDiscoveryReq") {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_ChargeParameterDiscoveryReqType(&body.ChargeParameterDiscoveryReq);
            body.ChargeParameterDiscoveryReq_isUsed = true;

            if (inJson["Body"]["ChargeParameterDiscoveryReq"].contains("MaxEntriesSAScheduleTuple")) {
                body.ChargeParameterDiscoveryReq.MaxEntriesSAScheduleTuple_isUsed = true;
                body.ChargeParameterDiscoveryReq.MaxEntriesSAScheduleTuple = inJson["Body"]["ChargeParameterDiscoveryReq"]["MaxEntriesSAScheduleTuple"].template get<uint8_t>();
            } else {
                body.ChargeParameterDiscoveryReq.MaxEntriesSAScheduleTuple_isUsed = false;
            }

            body.ChargeParameterDiscoveryReq.RequestedEnergyTransferMode = static_cast<iso2_EnergyTransferModeType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["RequestedEnergyTransferMode"].template get<int>());

            if (inJson["Body"]["ChargeParameterDiscoveryReq"].contains("AC_EVChargeParameter")) {
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter_isUsed = true;

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"].contains("DepartureTime")) {
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.DepartureTime_isUsed = true;
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.DepartureTime = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["DepartureTime"].template get<uint32_t>();
                }

                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EAmount.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EAmount"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EAmount.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EAmount"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EAmount.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EAmount"]["Value"].template get<uint32_t>();

                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxVoltage.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxVoltage.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxVoltage.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Value"].template get<uint32_t>();

                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxCurrent.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxCurrent.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxCurrent.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Value"].template get<uint32_t>();

                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMinCurrent.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMinCurrent"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMinCurrent.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMinCurrent"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMinCurrent.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMinCurrent"]["Value"].template get<uint32_t>();
            } else {
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter_isUsed = false;
            }

            if (inJson["Body"]["ChargeParameterDiscoveryReq"].contains("DC_EVChargeParameter")) {
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter_isUsed = true;
                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"].contains("DepartureTime")) {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DepartureTime_isUsed = true;
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DepartureTime = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DepartureTime"].template get<uint32_t>();
                }
                
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVReady = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVReady"].template get<bool>();
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVErrorCode = static_cast<iso2_DC_EVErrorCodeType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVErrorCode"].template get<int>());
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVRESSSOC = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVRESSSOC"].template get<uint8_t>();

                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Value"].template get<uint32_t>();

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"].contains("EVMaximumPowerLimit")) {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit_isUsed = true;
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Multiplier"].template get<int8_t>();
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Unit"].template get<int>());
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Value"].template get<uint32_t>();
                } else {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit_isUsed = false;
                }

                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Value"].template get<uint32_t>();

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"].contains("EVEnergyCapacity")) {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity_isUsed = true;
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Multiplier"].template get<int8_t>();
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Unit"].template get<int>());
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Value"].template get<uint32_t>();
                } else {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity_isUsed = false;
                }

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"].contains("FullSOC")) {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.FullSOC_isUsed = true;
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.FullSOC = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["FullSOC"].template get<uint8_t>();
                } else {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.FullSOC_isUsed = false;
                }

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"].contains("BulkSOC")) {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.BulkSOC_isUsed = true;
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.BulkSOC = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["BulkSOC"].template get<uint8_t>();
                } else {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.BulkSOC_isUsed = false;
                }
            } else {
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter_isUsed = false;
            }

            if (inJson["Body"]["ChargeParameterDiscoveryReq"].contains("EVChargeParameter")) {
                body.ChargeParameterDiscoveryReq.EVChargeParameter_isUsed = true;
                
                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"].contains("DepartureTime")) {
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.DepartureTime_isUsed = true;
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.DepartureTime = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["DepartureTime"].template get<uint32_t>();
                } else {
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.DepartureTime_isUsed = false;
                }

                body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EAmount.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EAmount"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EAmount.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EAmount"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EAmount.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EAmount"]["Value"].template get<uint32_t>();

                body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMaxVoltage.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMaxVoltage.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMaxVoltage.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Value"].template get<uint32_t>();

                body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMaxCurrent.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMaxCurrent.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMaxCurrent.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Value"].template get<uint32_t>();

                body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMinCurrent.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMinCurrent"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMinCurrent.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMinCurrent"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryReq.EVChargeParameter.AC_EVChargeParameter.EVMinCurrent.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["AC_EVChargeParameter"]["EVMinCurrent"]["Value"].template get<uint32_t>();

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"].contains("DepartureTime")) {
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.DepartureTime_isUsed = true;
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.DepartureTime = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["DepartureTime"].template get<uint32_t>();
                } else {
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.DepartureTime_isUsed = false;
                }

                body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.DC_EVStatus.EVReady = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVReady"].template get<bool>();
                body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.DC_EVStatus.EVErrorCode = static_cast<iso2_DC_EVErrorCodeType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVErrorCode"].template get<int>());
                body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.DC_EVStatus.EVRESSSOC = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVRESSSOC"].template get<uint8_t>();

                body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumCurrentLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumCurrentLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumCurrentLimit.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Value"].template get<uint32_t>();

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"].contains("EVMaximumPowerLimit")) {
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumPowerLimit_isUsed = true;
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumPowerLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Multiplier"].template get<int8_t>();
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumPowerLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Unit"].template get<int>());
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumPowerLimit.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Value"].template get<uint32_t>();
                } else {
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumPowerLimit_isUsed = false;
                }

                body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumVoltageLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumVoltageLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVMaximumVoltageLimit.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Value"].template get<uint32_t>();

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"].contains("EVEnergyCapacity")) {
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVEnergyCapacity_isUsed = true;
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVEnergyCapacity.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Multiplier"].template get<int8_t>();
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVEnergyCapacity.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Unit"].template get<int>());
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVEnergyCapacity.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Value"].template get<uint32_t>();
                } else {
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.EVEnergyCapacity_isUsed = false;
                }

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"].contains("FullSOC")) {
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.FullSOC_isUsed = true;
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.FullSOC = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["FullSOC"].template get<uint8_t>();
                } else {
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.FullSOC_isUsed = false;
                }

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"].contains("BulkSOC")) {
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.BulkSOC_isUsed = true;
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.BulkSOC = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter"]["DC_EVChargeParameter"]["BulkSOC"].template get<uint8_t>();
                } else {
                    body.ChargeParameterDiscoveryReq.EVChargeParameter.DC_EVChargeParameter.BulkSOC_isUsed = false;
                }
            }
        } else if (packetType == "ChargeParameterDiscoveryRes") {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_ChargeParameterDiscoveryResType(&body.ChargeParameterDiscoveryRes);
            body.ChargeParameterDiscoveryRes_isUsed = true;

            body.ChargeParameterDiscoveryRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["ResponseCode"].template get<int>());

            if (inJson["Body"]["ChargeParameterDiscoveryRes"].contains("SAScheduleList")) {
                body.ChargeParameterDiscoveryRes.SAScheduleList_isUsed = true;
                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.arrayLen = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["arrayLen"].template get<uint8_t>();
                for (int i = 0; i < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.arrayLen; i++) {
                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SAScheduleTupleID = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SAScheduleTupleID"].template get<uint8_t>();
                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.arrayLen = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["arrayLen"].template get<uint8_t>();
                    for (int j = 0; j < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.arrayLen; j++) {
                        if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j].contains("RelativeTimeInterval")) {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval_isUsed = true;
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.start = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["RelativeTimeInterval"]["start"].template get<uint32_t>();
                            if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["RelativeTimeInterval"].contains("duration")) {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.duration_isUsed = true;
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.duration = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["RelativeTimeInterval"]["duration"].template get<uint32_t>();
                            }
                        }
                        if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j].contains("TimeInterval")) {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].TimeInterval_isUsed = true;
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].TimeInterval._unused = 0;
                        }
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].PMax.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["PMax"]["Multiplier"].template get<int8_t>();
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].PMax.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["PMax"]["Unit"].template get<int>());
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].PMax.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["PMax"]["Value"].template get<uint32_t>();
                    }
                    if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i].contains("SalesTariff")) {
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff_isUsed = true;
                        if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"].contains("Id")) {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id_isUsed = true;
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id.charactersLen = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["Id"]["charactersLen"].template get<uint16_t>();
                            for (int k = 0; k < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id.charactersLen; k++) {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id.characters[k] = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["Id"]["characters"][k].template get<char>();
                            }
                        } else {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id_isUsed = false;
                        }
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffID = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffID"].template get<uint32_t>();
                        if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"].contains("SalesTariffDescription")) {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription_isUsed = true;
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription.charactersLen = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffDescription"]["charactersLen"].template get<uint16_t>();
                            for (int k = 0; k < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription.charactersLen; k++) {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription.characters[k] = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffDescription"]["characters"][k].template get<char>();
                            }
                        } else {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription_isUsed = false;
                        }
                        if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"].contains("NumEPriceLevels")) {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.NumEPriceLevels_isUsed = true;
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.NumEPriceLevels = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["NumEPriceLevels"].template get<uint8_t>();
                        } else {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.NumEPriceLevels_isUsed = false;
                        }
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.arrayLen = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["arrayLen"].template get<uint8_t>();
                        for (int j = 0; j < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.arrayLen; j++) {
                            if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j].contains("RelativeTimeInterval")) {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval_isUsed = true;
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval.start = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["RelativeTimeInterval"]["start"].template get<uint32_t>();
                                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["RelativeTimeInterval"].contains("duration")) {
                                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval.duration_isUsed = true;
                                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval.duration = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["RelativeTimeInterval"]["duration"].template get<uint32_t>();
                                } else {
                                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval.duration_isUsed = false;
                                }
                            } else {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval_isUsed = false;
                            }
                            if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j].contains("TimeInterval")) {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].TimeInterval_isUsed = true;
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].TimeInterval._unused = 0;
                            } else {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].TimeInterval_isUsed = false;
                            }
                            if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j].contains("EPriceLevel")) {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].EPriceLevel_isUsed = true;
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].EPriceLevel = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["EPriceLevel"].template get<uint8_t>();
                            } else {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].EPriceLevel_isUsed = false;
                            }
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.arrayLen = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["arrayLen"].template get<uint8_t>();
                            for (int k = 0; k < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.arrayLen; k++) {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].startValue.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["startValue"]["Multiplier"].template get<int8_t>();
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].startValue.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["startValue"]["Unit"].template get<int>());
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].startValue.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["startValue"]["Value"].template get<uint32_t>();
                                
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.arrayLen = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["Cost"]["arrayLen"].template get<uint8_t>();
                                for (int l = 0; l < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.arrayLen; l++) {
                                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.array[l].costKind = static_cast<iso2_costKindType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["Cost"]["array"][l]["costKind"].template get<int>());
                                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.array[l].amount = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["Cost"]["array"][l]["amount"].template get<uint32_t>();
                                    if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["Cost"]["array"][l].contains("amountMultiplier")) {
                                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.array[l].amountMultiplier_isUsed = true;
                                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.array[l].amountMultiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["array"][k]["Cost"]["array"][l]["amountMultiplier"].template get<int8_t>();
                                    } else {
                                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.array[k].Cost.array[l].amountMultiplier_isUsed = false;
                                    }
                                }
                            }
                        }
                    } else {
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff_isUsed = false;
                    }
                }
            } else {
                body.ChargeParameterDiscoveryRes.SAScheduleList_isUsed = false;
            }
            if (inJson["Body"]["ChargeParameterDiscoveryRes"].contains("SASchedules")) {
                body.ChargeParameterDiscoveryRes.SASchedules_isUsed = true;
                body.ChargeParameterDiscoveryRes.SASchedules._unused = 0;
            } else {
                body.ChargeParameterDiscoveryRes.SASchedules_isUsed = false;
            }

            if (inJson["Body"]["ChargeParameterDiscoveryRes"].contains("AC_EVSEChargeParameter")) {
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter_isUsed = true;

                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["EVSENotification"].template get<int>());
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.RCD = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["RCD"].template get<bool>();

                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSENominalVoltage.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSENominalVoltage"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSENominalVoltage.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSENominalVoltage"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSENominalVoltage.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSENominalVoltage"]["Value"].template get<int16_t>();

                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"]["Value"].template get<int16_t>();
            }

            if (inJson["Body"]["ChargeParameterDiscoveryRes"].contains("DC_EVSEChargeParameter")) {
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter_isUsed = true;

                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());
                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEIsolationStatus = static_cast<iso2_isolationLevelType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
                }
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEStatusCode = static_cast<iso2_DC_EVSEStatusCodeType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());

                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"]["Value"].template get<int16_t>();

                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"]["Value"].template get<int16_t>();

                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"]["Value"].template get<int16_t>();

                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"]["Value"].template get<int16_t>();

                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"]["Value"].template get<int16_t>();

                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"].contains("EVSECurrentRegulationTolerance")) {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance_isUsed = true;

                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"]["Multiplier"].template get<int8_t>();
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"]["Unit"].template get<int>());
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"]["Value"].template get<uint16_t>();
                } else {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance_isUsed = false;
                }

                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"]["Multiplier"].template get<int8_t>();
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"]["Unit"].template get<int>());
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"]["Value"].template get<uint16_t>();

                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"].contains("EVSEEnergyToBeDelivered")) {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered_isUsed = true;

                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"]["Multiplier"].template get<int8_t>();
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"]["Unit"].template get<int>());
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"]["Value"].template get<uint32_t>();
                } else {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered_isUsed = false;
                }
            } else {
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter_isUsed = false;
            }

            if (inJson["Body"]["ChargeParameterDiscoveryRes"].contains("EVSEChargeParameter")) {
                body.ChargeParameterDiscoveryRes.EVSEChargeParameter_isUsed = true;
                body.ChargeParameterDiscoveryRes.EVSEChargeParameter._unused = 0;
            } else {
                body.ChargeParameterDiscoveryRes.EVSEChargeParameter_isUsed = false;
            }
        } else if (packetType == "ChargingStatusReq") {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_ChargingStatusReqType(&body.ChargingStatusReq);
            body.ChargingStatusReq_isUsed = true;

            body.ChargingStatusReq._unused = 0;
        } else if (packetType == "ChargingStatusRes") {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_ChargingStatusResType(&body.ChargingStatusRes);
            body.ChargingStatusRes_isUsed = true;

            body.ChargingStatusRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["ChargingStatusRes"]["ResponseCode"].template get<int>());

            body.ChargingStatusRes.EVSEID.charactersLen = inJson["Body"]["ChargingStatusRes"]["EVSEID"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.ChargingStatusRes.EVSEID.charactersLen; i++) {
                body.ChargingStatusRes.EVSEID.characters[i] = inJson["Body"]["ChargingStatusRes"]["EVSEID"]["characters"][i].template get<char>();
            }

            if (inJson["Body"]["ChargingStatusRes"].contains("EVSEMaxCurrent")) {
                body.ChargingStatusRes.EVSEMaxCurrent_isUsed = true;
                body.ChargingStatusRes.EVSEMaxCurrent.Multiplier = inJson["Body"]["ChargingStatusRes"]["EVSEMaxCurrent"]["Multiplier"].template get<int8_t>();
                body.ChargingStatusRes.EVSEMaxCurrent.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ChargingStatusRes"]["EVSEMaxCurrent"]["Unit"].template get<int>());
                body.ChargingStatusRes.EVSEMaxCurrent.Value = inJson["Body"]["ChargingStatusRes"]["EVSEMaxCurrent"]["Value"].template get<uint16_t>();
            } else {
                body.ChargingStatusRes.EVSEMaxCurrent_isUsed = false;
            }

            if (inJson["Body"]["ChargingStatusRes"].contains("MeterInfo")) {
                body.ChargingStatusRes.MeterInfo_isUsed = true;
                body.ChargingStatusRes.MeterInfo.MeterID.charactersLen = inJson["Body"]["ChargingStatusRes"]["MeterInfo"]["MeterID"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.ChargingStatusRes.MeterInfo.MeterID.charactersLen; i++) {
                    body.ChargingStatusRes.MeterInfo.MeterID.characters[i] = inJson["Body"]["ChargingStatusRes"]["MeterInfo"]["MeterID"]["characters"][i].template get<char>();
                }

                if (inJson["Body"]["ChargingStatusRes"]["MeterInfo"].contains("MeterReading")) {
                    body.ChargingStatusRes.MeterInfo.MeterReading_isUsed = true;
                    body.ChargingStatusRes.MeterInfo.MeterReading = inJson["Body"]["ChargingStatusRes"]["MeterInfo"]["MeterReading"].template get<uint64_t>();
                } else {
                    body.ChargingStatusRes.MeterInfo.MeterReading_isUsed = false;
                }

                if (inJson["Body"]["ChargingStatusRes"]["MeterInfo"].contains("SigMeterReading")) {
                    body.ChargingStatusRes.MeterInfo.SigMeterReading_isUsed = true;
                    body.ChargingStatusRes.MeterInfo.SigMeterReading.bytesLen = inJson["Body"]["ChargingStatusRes"]["MeterInfo"]["SigMeterReading"]["bytesLen"].template get<uint16_t>();
                    for (int i = 0; i < body.ChargingStatusRes.MeterInfo.SigMeterReading.bytesLen; i++) {
                        body.ChargingStatusRes.MeterInfo.SigMeterReading.bytes[i] = inJson["Body"]["ChargingStatusRes"]["MeterInfo"]["SigMeterReading"]["bytes"][i].template get<uint8_t>();
                    }
                } else {
                    body.ChargingStatusRes.MeterInfo.SigMeterReading_isUsed = false;
                }

                if (inJson["Body"]["ChargingStatusRes"]["MeterInfo"].contains("MeterStatus")) {
                    body.ChargingStatusRes.MeterInfo.MeterStatus_isUsed = true;
                    body.ChargingStatusRes.MeterInfo.MeterStatus = inJson["Body"]["ChargingStatusRes"]["MeterInfo"]["MeterStatus"].template get<int16_t>();
                } else {
                    body.ChargingStatusRes.MeterInfo.MeterStatus_isUsed = false;
                }
                
                if (inJson["Body"]["ChargingStatusRes"]["MeterInfo"].contains("TMeter")) {
                    body.ChargingStatusRes.MeterInfo.TMeter_isUsed = true;
                    body.ChargingStatusRes.MeterInfo.TMeter = inJson["Body"]["ChargingStatusRes"]["MeterInfo"]["TMeter"].template get<int64_t>();
                } else {
                    body.ChargingStatusRes.MeterInfo.TMeter_isUsed = false;
                }
            } else {
                body.ChargingStatusRes.MeterInfo_isUsed = false;
            }

            if (inJson["Body"]["ChargingStatusRes"].contains("ReceiptRequired")) {
                body.ChargingStatusRes.ReceiptRequired_isUsed = true;
                body.ChargingStatusRes.ReceiptRequired = inJson["Body"]["ChargingStatusRes"]["ReceiptRequired"].template get<bool>();
            } else {
                body.ChargingStatusRes.ReceiptRequired_isUsed = false;
            }

            body.ChargingStatusRes.AC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["ChargingStatusRes"]["AC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
            body.ChargingStatusRes.AC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["ChargingStatusRes"]["AC_EVSEStatus"]["EVSENotification"].template get<int>());
            body.ChargingStatusRes.AC_EVSEStatus.RCD = inJson["Body"]["ChargingStatusRes"]["AC_EVSEStatus"]["RCD"].template get<bool>();
        } else if (packetType == "CurrentDemandReq") {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_CurrentDemandReqType(&body.CurrentDemandReq);
            body.CurrentDemandReq_isUsed = true;

            body.CurrentDemandReq.DC_EVStatus.EVReady = inJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVReady"].template get<bool>();
            body.CurrentDemandReq.DC_EVStatus.EVErrorCode = static_cast<iso2_DC_EVErrorCodeType>(inJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVErrorCode"].template get<int>());
            body.CurrentDemandReq.DC_EVStatus.EVRESSSOC = inJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVRESSSOC"].template get<uint8_t>();

            body.CurrentDemandReq.EVTargetCurrent.Multiplier = inJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"]["Multiplier"].template get<int8_t>();
            body.CurrentDemandReq.EVTargetCurrent.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"]["Unit"].template get<int>());
            body.CurrentDemandReq.EVTargetCurrent.Value = inJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"]["Value"].template get<uint16_t>();

            if (inJson["Body"]["CurrentDemandReq"].contains("EVMaximumVoltageLimit")) {
                body.CurrentDemandReq.EVMaximumVoltageLimit_isUsed = true;

                body.CurrentDemandReq.EVMaximumVoltageLimit.Multiplier = inJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"]["Multiplier"].template get<int8_t>();
                body.CurrentDemandReq.EVMaximumVoltageLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"]["Unit"].template get<int>());
                body.CurrentDemandReq.EVMaximumVoltageLimit.Value = inJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"]["Value"].template get<int16_t>();
            } else {
                body.CurrentDemandReq.EVMaximumVoltageLimit_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandReq"].contains("EVMaximumCurrentLimit")) {
                body.CurrentDemandReq.EVMaximumCurrentLimit_isUsed = true;

                body.CurrentDemandReq.EVMaximumCurrentLimit.Multiplier = inJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"]["Multiplier"].template get<int8_t>();
                body.CurrentDemandReq.EVMaximumCurrentLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"]["Unit"].template get<int>());
                body.CurrentDemandReq.EVMaximumCurrentLimit.Value = inJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"]["Value"].template get<int16_t>();
            } else {
                body.CurrentDemandReq.EVMaximumCurrentLimit_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandReq"].contains("EVMaximumPowerLimit")) {
                body.CurrentDemandReq.EVMaximumPowerLimit_isUsed = true;

                body.CurrentDemandReq.EVMaximumPowerLimit.Multiplier = inJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"]["Multiplier"].template get<int8_t>();
                body.CurrentDemandReq.EVMaximumPowerLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"]["Unit"].template get<int>());
                body.CurrentDemandReq.EVMaximumPowerLimit.Value = inJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"]["Value"].template get<int16_t>();
            } else {
                body.CurrentDemandReq.EVMaximumPowerLimit_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandReq"].contains("BulkChargingComplete")) {
                body.CurrentDemandReq.BulkChargingComplete_isUsed = true;
                body.CurrentDemandReq.BulkChargingComplete = inJson["Body"]["CurrentDemandReq"]["BulkChargingComplete"].template get<bool>();
            } else {
                body.CurrentDemandReq.BulkChargingComplete_isUsed = false;
            }

            body.CurrentDemandReq.ChargingComplete = inJson["Body"]["CurrentDemandReq"]["ChargingComplete"].template get<bool>();

            if (inJson["Body"]["CurrentDemandReq"].contains("RemainingTimeToFullSoC")) {
                body.CurrentDemandReq.RemainingTimeToFullSoC_isUsed = true;
                
                body.CurrentDemandReq.RemainingTimeToFullSoC.Multiplier = inJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"]["Multiplier"].template get<int8_t>();
                body.CurrentDemandReq.RemainingTimeToFullSoC.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"]["Unit"].template get<int>());
                body.CurrentDemandReq.RemainingTimeToFullSoC.Value = inJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"]["Value"].template get<int16_t>();
            } else {
                body.CurrentDemandReq.RemainingTimeToFullSoC_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandReq"].contains("RemainingTimeToBulkSoC")) {
                body.CurrentDemandReq.RemainingTimeToBulkSoC_isUsed = true;
                
                body.CurrentDemandReq.RemainingTimeToBulkSoC.Multiplier = inJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"]["Multiplier"].template get<int8_t>();
                body.CurrentDemandReq.RemainingTimeToBulkSoC.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"]["Unit"].template get<int>());
                body.CurrentDemandReq.RemainingTimeToBulkSoC.Value = inJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"]["Value"].template get<int16_t>();
            } else {
                body.CurrentDemandReq.RemainingTimeToBulkSoC_isUsed = false;
            }

            body.CurrentDemandReq.EVTargetVoltage.Multiplier = inJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"]["Multiplier"].template get<int8_t>();
            body.CurrentDemandReq.EVTargetVoltage.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"]["Unit"].template get<int>());
            body.CurrentDemandReq.EVTargetVoltage.Value = inJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"]["Value"].template get<int16_t>();
        } else if (packetType == "CurrentDemandRes") {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_CurrentDemandResType(&body.CurrentDemandRes);
            body.CurrentDemandRes_isUsed = true;

            body.CurrentDemandRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["CurrentDemandRes"]["ResponseCode"].template get<int>());

            body.CurrentDemandRes.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
            body.CurrentDemandRes.DC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());
            if (inJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                body.CurrentDemandRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                body.CurrentDemandRes.DC_EVSEStatus.EVSEIsolationStatus = static_cast<iso2_isolationLevelType>(inJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
            } else {
                body.CurrentDemandRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
            }
            body.CurrentDemandRes.DC_EVSEStatus.EVSEStatusCode = static_cast<iso2_DC_EVSEStatusCodeType>(inJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());

            body.CurrentDemandRes.EVSEPresentVoltage.Multiplier = inJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"]["Multiplier"].template get<int8_t>();
            body.CurrentDemandRes.EVSEPresentVoltage.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"]["Unit"].template get<int>());
            body.CurrentDemandRes.EVSEPresentVoltage.Value = inJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"]["Value"].template get<int16_t>();

            body.CurrentDemandRes.EVSEPresentCurrent.Multiplier = inJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"]["Multiplier"].template get<int8_t>();
            body.CurrentDemandRes.EVSEPresentCurrent.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"]["Unit"].template get<int>());
            body.CurrentDemandRes.EVSEPresentCurrent.Value = inJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"]["Value"].template get<int16_t>();

            body.CurrentDemandRes.EVSECurrentLimitAchieved = inJson["Body"]["CurrentDemandRes"]["EVSECurrentLimitAchieved"].template get<bool>();
            body.CurrentDemandRes.EVSEVoltageLimitAchieved = inJson["Body"]["CurrentDemandRes"]["EVSEVoltageLimitAchieved"].template get<bool>();
            body.CurrentDemandRes.EVSEPowerLimitAchieved = inJson["Body"]["CurrentDemandRes"]["EVSEPowerLimitAchieved"].template get<bool>();

            if (inJson["Body"]["CurrentDemandRes"].contains("EVSEMaximumVoltageLimit")) {
                body.CurrentDemandRes.EVSEMaximumVoltageLimit_isUsed = true;

                body.CurrentDemandRes.EVSEMaximumVoltageLimit.Multiplier = inJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"]["Multiplier"].template get<int8_t>();
                body.CurrentDemandRes.EVSEMaximumVoltageLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"]["Unit"].template get<int>());
                body.CurrentDemandRes.EVSEMaximumVoltageLimit.Value = inJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"]["Value"].template get<int16_t>();
            } else {
                body.CurrentDemandRes.EVSEMaximumVoltageLimit_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandRes"].contains("EVSEMaximumCurrentLimit")) {
                body.CurrentDemandRes.EVSEMaximumCurrentLimit_isUsed = true;

                body.CurrentDemandRes.EVSEMaximumCurrentLimit.Multiplier = inJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"]["Multiplier"].template get<int8_t>();
                body.CurrentDemandRes.EVSEMaximumCurrentLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"]["Unit"].template get<int>());
                body.CurrentDemandRes.EVSEMaximumCurrentLimit.Value = inJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"]["Value"].template get<int16_t>();
            } else {
                body.CurrentDemandRes.EVSEMaximumCurrentLimit_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandRes"].contains("EVSEMaximumPowerLimit")) {
                body.CurrentDemandRes.EVSEMaximumPowerLimit_isUsed = true;

                body.CurrentDemandRes.EVSEMaximumPowerLimit.Multiplier = inJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"]["Multiplier"].template get<int8_t>();
                body.CurrentDemandRes.EVSEMaximumPowerLimit.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"]["Unit"].template get<int>());
                body.CurrentDemandRes.EVSEMaximumPowerLimit.Value = inJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"]["Value"].template get<int16_t>();
            } else {
                body.CurrentDemandRes.EVSEMaximumPowerLimit_isUsed = false;
            }

            body.CurrentDemandRes.EVSEID.charactersLen = inJson["Body"]["CurrentDemandRes"]["EVSEID"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.CurrentDemandRes.EVSEID.charactersLen; i++) {
                body.CurrentDemandRes.EVSEID.characters[i] = inJson["Body"]["CurrentDemandRes"]["EVSEID"]["characters"][i].template get<char>();
            }

            body.CurrentDemandRes.SAScheduleTupleID = inJson["Body"]["CurrentDemandRes"]["SAScheduleTupleID"].template get<uint8_t>();

            if (inJson["Body"]["CurrentDemandRes"].contains("MeterInfo")) {
                body.CurrentDemandRes.MeterInfo_isUsed = true;
                body.CurrentDemandRes.MeterInfo.MeterID.charactersLen = inJson["Body"]["CurrentDemandRes"]["MeterInfo"]["MeterID"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.CurrentDemandRes.MeterInfo.MeterID.charactersLen; i++) {
                    body.CurrentDemandRes.MeterInfo.MeterID.characters[i] = inJson["Body"]["CurrentDemandRes"]["MeterInfo"]["MeterID"]["characters"][i].template get<char>();
                }

                if (inJson["Body"]["CurrentDemandRes"]["MeterInfo"].contains("MeterReading")) {
                    body.CurrentDemandRes.MeterInfo.MeterReading_isUsed = true;
                    body.CurrentDemandRes.MeterInfo.MeterReading = inJson["Body"]["CurrentDemandRes"]["MeterInfo"]["MeterReading"].template get<uint64_t>();
                } else {
                    body.CurrentDemandRes.MeterInfo.MeterReading_isUsed = false;
                }

                if (inJson["Body"]["CurrentDemandRes"]["MeterInfo"].contains("SigMeterReading")) {
                    body.CurrentDemandRes.MeterInfo.SigMeterReading_isUsed = true;
                    body.CurrentDemandRes.MeterInfo.SigMeterReading.bytesLen = inJson["Body"]["CurrentDemandRes"]["MeterInfo"]["SigMeterReading"]["bytesLen"].template get<uint16_t>();
                    for (int i = 0; i < body.CurrentDemandRes.MeterInfo.SigMeterReading.bytesLen; i++) {
                        body.CurrentDemandRes.MeterInfo.SigMeterReading.bytes[i] = inJson["Body"]["CurrentDemandRes"]["MeterInfo"]["SigMeterReading"]["bytes"][i].template get<uint8_t>();
                    }
                } else {
                    body.CurrentDemandRes.MeterInfo.SigMeterReading_isUsed = false;
                }

                if (inJson["Body"]["CurrentDemandRes"]["MeterInfo"].contains("MeterStatus")) {
                    body.CurrentDemandRes.MeterInfo.MeterStatus_isUsed = true;
                    body.CurrentDemandRes.MeterInfo.MeterStatus = inJson["Body"]["CurrentDemandRes"]["MeterInfo"]["MeterStatus"].template get<int16_t>();
                } else {
                    body.CurrentDemandRes.MeterInfo.MeterStatus_isUsed = false;
                }

                if (inJson["Body"]["CurrentDemandRes"]["MeterInfo"].contains("TMeter")) {
                    body.CurrentDemandRes.MeterInfo.TMeter_isUsed = true;
                    body.CurrentDemandRes.MeterInfo.TMeter = inJson["Body"]["CurrentDemandRes"]["MeterInfo"]["TMeter"].template get<int64_t>();
                } else {
                    body.CurrentDemandRes.MeterInfo.TMeter_isUsed = false;
                }
            } else {
                body.CurrentDemandRes.MeterInfo_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandRes"].contains("ReceiptRequired")) {
                body.CurrentDemandRes.ReceiptRequired_isUsed = true;
                body.CurrentDemandRes.ReceiptRequired = inJson["Body"]["CurrentDemandRes"]["ReceiptRequired"].template get<bool>();
            } else {
                body.CurrentDemandRes.ReceiptRequired_isUsed = false;
            }
        } else if (packetType == "MeteringReceiptReq") {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_MeteringReceiptReqType(&body.MeteringReceiptReq);
            body.MeteringReceiptReq_isUsed = true;

            if (inJson["Body"]["MeteringReceiptReq"].contains("Id")) {
                body.MeteringReceiptReq.Id_isUsed = true;
                body.MeteringReceiptReq.Id.charactersLen = inJson["Body"]["MeteringReceiptReq"]["Id"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.MeteringReceiptReq.Id.charactersLen; i++) {
                    body.MeteringReceiptReq.Id.characters[i] = inJson["Body"]["MeteringReceiptReq"]["Id"]["characters"][i].template get<char>();
                }
            } else {
                body.MeteringReceiptReq.Id_isUsed = false;
            }

            body.MeteringReceiptReq.SessionID.bytesLen = inJson["Body"]["MeteringReceiptReq"]["SessionID"]["bytesLen"].template get<uint8_t>();
            for (int i = 0; i < body.MeteringReceiptReq.SessionID.bytesLen; i++) {
                body.MeteringReceiptReq.SessionID.bytes[i] = inJson["Body"]["MeteringReceiptReq"]["SessionID"]["bytes"][i].template get<uint8_t>();
            }

            if (inJson["Body"]["MeteringReceiptReq"].contains("SAScheduleTupleID")) {
                body.MeteringReceiptReq.SAScheduleTupleID_isUsed = true;
                body.MeteringReceiptReq.SAScheduleTupleID = inJson["Body"]["MeteringReceiptReq"]["SAScheduleTupleID"].template get<uint8_t>();
            } else {
                body.MeteringReceiptReq.SAScheduleTupleID_isUsed = false;
            }

            body.MeteringReceiptReq.MeterInfo.MeterID.charactersLen = inJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["MeterID"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.MeteringReceiptReq.MeterInfo.MeterID.charactersLen; i++) {
                body.MeteringReceiptReq.MeterInfo.MeterID.characters[i] = inJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["MeterID"]["characters"][i].template get<char>();
            }

            if (inJson["Body"]["MeteringReceiptReq"]["MeterInfo"].contains("MeterReading")) {
                body.MeteringReceiptReq.MeterInfo.MeterReading_isUsed = true;
                body.MeteringReceiptReq.MeterInfo.MeterReading = inJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["MeterReading"].template get<uint64_t>();
            } else {
                body.MeteringReceiptReq.MeterInfo.MeterReading_isUsed = false;
            }

            if (inJson["Body"]["MeteringReceiptReq"]["MeterInfo"].contains("SigMeterReading")) {
                body.MeteringReceiptReq.MeterInfo.SigMeterReading_isUsed = true;
                body.MeteringReceiptReq.MeterInfo.SigMeterReading.bytesLen = inJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["SigMeterReading"]["bytesLen"].template get<uint16_t>();
                for (int i = 0; i < body.MeteringReceiptReq.MeterInfo.SigMeterReading.bytesLen; i++) {
                    body.MeteringReceiptReq.MeterInfo.SigMeterReading.bytes[i] = inJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["SigMeterReading"]["bytes"][i].template get<uint8_t>();
                }
            } else {
                body.MeteringReceiptReq.MeterInfo.SigMeterReading_isUsed = false;
            }

            if (inJson["Body"]["MeteringReceiptReq"]["MeterInfo"].contains("MeterStatus")) {
                body.MeteringReceiptReq.MeterInfo.MeterStatus_isUsed = true;
                body.MeteringReceiptReq.MeterInfo.MeterStatus = inJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["MeterStatus"].template get<int16_t>();
            } else {
                body.MeteringReceiptReq.MeterInfo.MeterStatus_isUsed = false;
            }

            if (inJson["Body"]["MeteringReceiptReq"]["MeterInfo"].contains("TMeter")) {
                body.MeteringReceiptReq.MeterInfo.TMeter_isUsed = true;
                body.MeteringReceiptReq.MeterInfo.TMeter = inJson["Body"]["MeteringReceiptReq"]["MeterInfo"]["TMeter"].template get<int64_t>();
            } else {
                body.MeteringReceiptReq.MeterInfo.TMeter_isUsed = false;
            }
        } else if (packetType == "MeteringReceiptRes") {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_MeteringReceiptResType(&body.MeteringReceiptRes);
            body.MeteringReceiptRes_isUsed = true;

            body.MeteringReceiptRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["MeteringReceiptRes"]["ResponseCode"].template get<int>());

            if (inJson["Body"]["MeteringReceiptRes"].contains("AC_EVSEStatus")) {
                body.MeteringReceiptRes.AC_EVSEStatus_isUsed = true;

                body.MeteringReceiptRes.AC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["MeteringReceiptRes"]["AC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
                body.MeteringReceiptRes.AC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["MeteringReceiptRes"]["AC_EVSEStatus"]["EVSENotification"].template get<int>());
                body.MeteringReceiptRes.AC_EVSEStatus.RCD = inJson["Body"]["MeteringReceiptRes"]["AC_EVSEStatus"]["RCD"].template get<bool>();
            } else {
                body.MeteringReceiptRes.AC_EVSEStatus_isUsed = false;
            }

            if (inJson["Body"]["MeteringReceiptRes"].contains("DC_EVSEStatus")) {
                body.MeteringReceiptRes.DC_EVSEStatus_isUsed = true;

                body.MeteringReceiptRes.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["MeteringReceiptRes"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
                body.MeteringReceiptRes.DC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["MeteringReceiptRes"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());
                if (inJson["Body"]["MeteringReceiptRes"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                    body.MeteringReceiptRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                    body.MeteringReceiptRes.DC_EVSEStatus.EVSEIsolationStatus = static_cast<iso2_isolationLevelType>(inJson["Body"]["MeteringReceiptRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
                } else {
                    body.MeteringReceiptRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
                }
                body.MeteringReceiptRes.DC_EVSEStatus.EVSEStatusCode = static_cast<iso2_DC_EVSEStatusCodeType>(inJson["Body"]["MeteringReceiptRes"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());
            } else {
                body.MeteringReceiptRes.DC_EVSEStatus_isUsed = false;
            }

            if (inJson["Body"]["MeteringReceiptRes"].contains("EVSEStatus")) {
                body.MeteringReceiptRes.EVSEStatus_isUsed = true;
                
                body.MeteringReceiptRes.EVSEStatus.AC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["AC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
                body.MeteringReceiptRes.EVSEStatus.AC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["AC_EVSEStatus"]["EVSENotification"].template get<int>());
                body.MeteringReceiptRes.EVSEStatus.AC_EVSEStatus.RCD = inJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["AC_EVSEStatus"]["RCD"].template get<bool>();

                body.MeteringReceiptRes.EVSEStatus.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
                body.MeteringReceiptRes.EVSEStatus.DC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());
                if (inJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                    body.MeteringReceiptRes.EVSEStatus.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                    body.MeteringReceiptRes.EVSEStatus.DC_EVSEStatus.EVSEIsolationStatus = static_cast<iso2_isolationLevelType>(inJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
                } else {
                    body.MeteringReceiptRes.EVSEStatus.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
                }
                body.MeteringReceiptRes.EVSEStatus.DC_EVSEStatus.EVSEStatusCode = static_cast<iso2_DC_EVSEStatusCodeType>(inJson["Body"]["MeteringReceiptRes"]["EVSEStatus"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());
            } else {
                body.MeteringReceiptRes.EVSEStatus_isUsed = false;
            }
        } else if (packetType == "PaymentDetailsReq") {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_PaymentDetailsReqType(&body.PaymentDetailsReq);
            body.PaymentDetailsReq_isUsed = true;

            body.PaymentDetailsReq.eMAID.charactersLen = inJson["Body"]["PaymentDetailsReq"]["eMAID"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.PaymentDetailsReq.eMAID.charactersLen; i++) {
                body.PaymentDetailsReq.eMAID.characters[i] = inJson["Body"]["PaymentDetailsReq"]["eMAID"]["characters"][i].template get<char>();
            }

            if (inJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"].contains("Id")) {
                body.PaymentDetailsReq.ContractSignatureCertChain.Id_isUsed = true;
                body.PaymentDetailsReq.ContractSignatureCertChain.Id.charactersLen = inJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["Id"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.PaymentDetailsReq.ContractSignatureCertChain.Id.charactersLen; i++) {
                    body.PaymentDetailsReq.ContractSignatureCertChain.Id.characters[i] = inJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["Id"]["characters"][i].template get<char>();
                }
            } else {
                body.PaymentDetailsReq.ContractSignatureCertChain.Id_isUsed = false;
            }

            body.PaymentDetailsReq.ContractSignatureCertChain.Certificate.bytesLen = inJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["Certificate"]["bytesLen"].template get<uint16_t>();
            for (int i = 0; i < body.PaymentDetailsReq.ContractSignatureCertChain.Certificate.bytesLen; i++) {
                body.PaymentDetailsReq.ContractSignatureCertChain.Certificate.bytes[i] = inJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["Certificate"]["bytes"][i].template get<uint8_t>();
            }

            if (inJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"].contains("SubCertificates")) {
                body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates_isUsed = true;
                body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen = inJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["arrayLen"].template get<uint16_t>();
                for (int i = 0; i < body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates.Certificate.arrayLen; i++) {
                    body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen = inJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytesLen"].template get<uint16_t>();
                    for (int j = 0; j < body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytesLen; j++) {
                        body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates.Certificate.array[i].bytes[j] = inJson["Body"]["PaymentDetailsReq"]["ContractSignatureCertChain"]["SubCertificates"]["Certificate"]["array"][i]["bytes"][j].template get<uint8_t>();
                    }
                }
            } else {
                body.PaymentDetailsReq.ContractSignatureCertChain.SubCertificates_isUsed = false;
            }
        } else if (packetType == "PaymentDetailsRes") {/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_PaymentDetailsResType(&body.PaymentDetailsRes);
            body.PaymentDetailsRes_isUsed = true;

            body.PaymentDetailsRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["PaymentDetailsRes"]["ResponseCode"].template get<int>());

            body.PaymentDetailsRes.GenChallenge.bytesLen = inJson["Body"]["PaymentDetailsRes"]["GenChallenge"]["bytesLen"].template get<uint8_t>();
            for (int i = 0; i < body.PaymentDetailsRes.GenChallenge.bytesLen; i++) {
                body.PaymentDetailsRes.GenChallenge.bytes[i] = inJson["Body"]["PaymentDetailsRes"]["GenChallenge"]["bytes"][i].template get<uint8_t>();
            }

            body.PaymentDetailsRes.EVSETimeStamp = inJson["Body"]["PaymentDetailsRes"]["EVSETimeStamp"].template get<int64_t>();
        } else if (packetType == "PaymentServiceSelectionReq") {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_PaymentServiceSelectionReqType(&body.PaymentServiceSelectionReq);
            body.PaymentServiceSelectionReq_isUsed = true;

            body.PaymentServiceSelectionReq.SelectedPaymentOption = static_cast<iso2_paymentOptionType>(inJson["Body"]["PaymentServiceSelectionReq"]["SelectedPaymentOption"].template get<int>());

            body.PaymentServiceSelectionReq.SelectedServiceList.SelectedService.arrayLen = inJson["Body"]["PaymentServiceSelectionReq"]["SelectedServiceList"]["SelectedService"]["arrayLen"].template get<uint16_t>();
            for (int i = 0; i < body.PaymentServiceSelectionReq.SelectedServiceList.SelectedService.arrayLen; i++) {
                body.PaymentServiceSelectionReq.SelectedServiceList.SelectedService.array[i].ServiceID = inJson["Body"]["PaymentServiceSelectionReq"]["SelectedServiceList"]["SelectedService"]["array"][i]["ServiceID"].template get<uint8_t>();
                if (inJson["Body"]["PaymentServiceSelectionReq"]["SelectedServiceList"]["SelectedService"]["array"][i].contains("ParameterSetID")) {
                    body.PaymentServiceSelectionReq.SelectedServiceList.SelectedService.array[i].ParameterSetID_isUsed = true;
                    body.PaymentServiceSelectionReq.SelectedServiceList.SelectedService.array[i].ParameterSetID = inJson["Body"]["PaymentServiceSelectionReq"]["SelectedServiceList"]["SelectedService"]["array"][i]["ParameterSetID"].template get<uint8_t>();
                } else {
                    body.PaymentServiceSelectionReq.SelectedServiceList.SelectedService.array[i].ParameterSetID_isUsed = false;
                }
            }
        } else if (packetType == "PaymentServiceSelectionRes") {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_PaymentServiceSelectionResType(&body.PaymentServiceSelectionRes);
            body.PaymentServiceSelectionRes_isUsed = true;

            body.PaymentServiceSelectionRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["PaymentServiceSelectionRes"]["ResponseCode"].template get<int>());
        } else if (packetType == "PowerDeliveryReq") {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_PowerDeliveryReqType(&body.PowerDeliveryReq);
            body.PowerDeliveryReq_isUsed = true;

            body.PowerDeliveryReq.ChargeProgress = static_cast<iso2_chargeProgressType>(inJson["Body"]["PowerDeliveryReq"]["ChargeProgress"].template get<int>());

            body.PowerDeliveryReq.SAScheduleTupleID = inJson["Body"]["PowerDeliveryReq"]["SAScheduleTupleID"].template get<uint8_t>();

            if (inJson["Body"]["PowerDeliveryReq"].contains("ChargingProfile")) {
                body.PowerDeliveryReq.ChargingProfile_isUsed = true;

                body.PowerDeliveryReq.ChargingProfile.ProfileEntry.arrayLen = inJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["arrayLen"].template get<uint16_t>();
                for (int i = 0; i < body.PowerDeliveryReq.ChargingProfile.ProfileEntry.arrayLen; i++) {
                    body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryStart = inJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryStart"].template get<uint32_t>();

                    body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxPower.Multiplier = inJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryMaxPower"]["Multiplier"].template get<int8_t>();
                    body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxPower.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryMaxPower"]["Unit"].template get<int>());
                    body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxPower.Value = inJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryMaxPower"]["Value"].template get<int16_t>();

                    if (inJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i].contains("ChargingProfileEntryMaxNumberOfPhasesInUse")) {
                        body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxNumberOfPhasesInUse_isUsed = true;
                        body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxNumberOfPhasesInUse = inJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryMaxNumberOfPhasesInUse"].template get<uint8_t>();
                    } else {
                        body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxNumberOfPhasesInUse_isUsed = false;
                    }
                }
            } else {
                body.PowerDeliveryReq.ChargingProfile_isUsed = false;
            }

            if (inJson["Body"]["PowerDeliveryReq"].contains("DC_EVPowerDeliveryParameter")) {
                body.PowerDeliveryReq.DC_EVPowerDeliveryParameter_isUsed = true;

                body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVReady = inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVReady"].template get<bool>();
                body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVErrorCode = static_cast<iso2_DC_EVErrorCodeType>(inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVErrorCode"].template get<int>());
                body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVRESSSOC = inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVRESSSOC"].template get<uint8_t>();

                if (inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"].contains("BulkChargingComplete")) {
                    body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.BulkChargingComplete_isUsed = true;
                    body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.BulkChargingComplete = inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["BulkChargingComplete"].template get<bool>();
                } else {
                    body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.BulkChargingComplete_isUsed = false;
                }

                body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.ChargingComplete = inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["ChargingComplete"].template get<bool>();
            } else {
                body.PowerDeliveryReq.DC_EVPowerDeliveryParameter_isUsed = false;
            }

            if (inJson["Body"]["PowerDeliveryReq"].contains("EVPowerDeliveryParameter")) {
                body.PowerDeliveryReq.EVPowerDeliveryParameter_isUsed = true;

                body.PowerDeliveryReq.EVPowerDeliveryParameter._unused = 0;
            } else {    
                body.PowerDeliveryReq.EVPowerDeliveryParameter_isUsed = false;
            }
        } else if (packetType == "PowerDeliveryRes") {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_PowerDeliveryResType(&body.PowerDeliveryRes);
            body.PowerDeliveryRes_isUsed = true;

            body.PowerDeliveryRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["PowerDeliveryRes"]["ResponseCode"].template get<int>());

            if (inJson["Body"]["PowerDeliveryRes"].contains("AC_EVSEStatus")) {
                body.PowerDeliveryRes.AC_EVSEStatus_isUsed = true;

                body.PowerDeliveryRes.AC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
                body.PowerDeliveryRes.AC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["EVSENotification"].template get<int>());
                body.PowerDeliveryRes.AC_EVSEStatus.RCD = inJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["RCD"].template get<bool>();
            } else {
                body.PowerDeliveryRes.AC_EVSEStatus_isUsed = false;
            }

            if (inJson["Body"]["PowerDeliveryRes"].contains("DC_EVSEStatus")) {
                body.PowerDeliveryRes.DC_EVSEStatus_isUsed = true;

                body.PowerDeliveryRes.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
                body.PowerDeliveryRes.DC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());
                if (inJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                    body.PowerDeliveryRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                    body.PowerDeliveryRes.DC_EVSEStatus.EVSEIsolationStatus = static_cast<iso2_isolationLevelType>(inJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
                } else {
                    body.PowerDeliveryRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
                }
                body.PowerDeliveryRes.DC_EVSEStatus.EVSEStatusCode = static_cast<iso2_DC_EVSEStatusCodeType>(inJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());
            } else {
                body.PowerDeliveryRes.DC_EVSEStatus_isUsed = false;
            }

            if (inJson["Body"]["PowerDeliveryRes"].contains("EVSEStatus")) {
                body.PowerDeliveryRes.EVSEStatus_isUsed = true;
                
                body.PowerDeliveryRes.EVSEStatus.AC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["AC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
                body.PowerDeliveryRes.EVSEStatus.AC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["AC_EVSEStatus"]["EVSENotification"].template get<int>());
                body.PowerDeliveryRes.EVSEStatus.AC_EVSEStatus.RCD = inJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["AC_EVSEStatus"]["RCD"].template get<bool>();

                body.PowerDeliveryRes.EVSEStatus.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
                body.PowerDeliveryRes.EVSEStatus.DC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());
                if (inJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                    body.PowerDeliveryRes.EVSEStatus.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                    body.PowerDeliveryRes.EVSEStatus.DC_EVSEStatus.EVSEIsolationStatus = static_cast<iso2_isolationLevelType>(inJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
                } else {
                    body.PowerDeliveryRes.EVSEStatus.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
                }
                body.PowerDeliveryRes.EVSEStatus.DC_EVSEStatus.EVSEStatusCode = static_cast<iso2_DC_EVSEStatusCodeType>(inJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());
            } else {
                body.PowerDeliveryRes.EVSEStatus_isUsed = false;
            }
        } else if (packetType == "PreChargeReq") {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_PreChargeReqType(&body.PreChargeReq);
            body.PreChargeReq_isUsed = true;

            body.PreChargeReq.DC_EVStatus.EVReady = inJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVReady"].template get<bool>();
            body.PreChargeReq.DC_EVStatus.EVErrorCode = static_cast<iso2_DC_EVErrorCodeType>(inJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVErrorCode"].template get<int>());
            body.PreChargeReq.DC_EVStatus.EVRESSSOC = inJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVRESSSOC"].template get<uint8_t>();

            body.PreChargeReq.EVTargetVoltage.Multiplier = inJson["Body"]["PreChargeReq"]["EVTargetVoltage"]["Multiplier"].template get<int8_t>();
            body.PreChargeReq.EVTargetVoltage.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["PreChargeReq"]["EVTargetVoltage"]["Unit"].template get<int>());
            body.PreChargeReq.EVTargetVoltage.Value = inJson["Body"]["PreChargeReq"]["EVTargetVoltage"]["Value"].template get<int16_t>();

            body.PreChargeReq.EVTargetCurrent.Multiplier = inJson["Body"]["PreChargeReq"]["EVTargetCurrent"]["Multiplier"].template get<int8_t>();
            body.PreChargeReq.EVTargetCurrent.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["PreChargeReq"]["EVTargetCurrent"]["Unit"].template get<int>());
            body.PreChargeReq.EVTargetCurrent.Value = inJson["Body"]["PreChargeReq"]["EVTargetCurrent"]["Value"].template get<int16_t>();
        } else if (packetType == "PreChargeRes") {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_PreChargeResType(&body.PreChargeRes);
            body.PreChargeRes_isUsed = true;

            body.PreChargeRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["PreChargeRes"]["ResponseCode"].template get<int>());

            body.PreChargeRes.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
            body.PreChargeRes.DC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());
            if (inJson["Body"]["PreChargeRes"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                body.PreChargeRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                body.PreChargeRes.DC_EVSEStatus.EVSEIsolationStatus = static_cast<iso2_isolationLevelType>(inJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
            } else {
                body.PreChargeRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
            }
            body.PreChargeRes.DC_EVSEStatus.EVSEStatusCode = static_cast<iso2_DC_EVSEStatusCodeType>(inJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());

            body.PreChargeRes.EVSEPresentVoltage.Multiplier = inJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"]["Multiplier"].template get<int8_t>();
            body.PreChargeRes.EVSEPresentVoltage.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"]["Unit"].template get<int>());
            body.PreChargeRes.EVSEPresentVoltage.Value = inJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"]["Value"].template get<int16_t>();
        } else if (packetType == "ServiceDetailReq") {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_ServiceDetailReqType(&body.ServiceDetailReq);
            body.ServiceDetailReq_isUsed = true;

            body.ServiceDetailReq.ServiceID = inJson["Body"]["ServiceDetailReq"]["ServiceID"].template get<uint8_t>();
        } else if (packetType == "ServiceDetailRes") {//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_ServiceDetailResType(&body.ServiceDetailRes);
            body.ServiceDetailRes_isUsed = true;

            body.ServiceDetailRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["ServiceDetailRes"]["ResponseCode"].template get<int>());

            if (inJson["Body"]["ServiceDetailRes"].contains("ServiceParameterList")) {
                body.ServiceDetailRes.ServiceParameterList_isUsed = true;
                body.ServiceDetailRes.ServiceParameterList.ParameterSet.arrayLen = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["arrayLen"].template get<uint16_t>();
                for (int i = 0; i < body.ServiceDetailRes.ServiceParameterList.ParameterSet.arrayLen; i++) {
                    body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].ParameterSetID = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["ParameterSetID"].template get<uint8_t>();
                    body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.arrayLen = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["arrayLen"].template get<uint16_t>();
                    for (int j = 0; j < body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.arrayLen; j++) {
                        body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].Name.charactersLen = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["Name"]["charactersLen"].template get<uint16_t>();
                        for (int k = 0; k < body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].Name.charactersLen; k++) {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].Name.characters[k] = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["Name"]["characters"][k].template get<char>();
                        }
                        if (inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j].contains("boolValue")) {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].boolValue_isUsed = true;
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].boolValue = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["boolValue"].template get<bool>();
                        } else {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].boolValue_isUsed = false;
                        }
                        if (inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j].contains("byteValue")) {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].byteValue_isUsed = true;
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].byteValue = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["byteValue"].template get<uint8_t>();
                        } else {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].byteValue_isUsed = false;
                        }
                        if (inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j].contains("shortValue")) {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].shortValue_isUsed = true;
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].shortValue = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["shortValue"].template get<int16_t>();
                        } else {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].shortValue_isUsed = false;
                        }
                        if (inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j].contains("intValue")) {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].intValue_isUsed = true;
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].intValue = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["intValue"].template get<int32_t>();
                        } else {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].intValue_isUsed = false;
                        }
                        if (inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j].contains("physicalValue")) {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].physicalValue_isUsed = true;
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].physicalValue.Multiplier = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["physicalValue"]["Multiplier"].template get<int8_t>();
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].physicalValue.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["physicalValue"]["Unit"].template get<int>());
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].physicalValue.Value = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["physicalValue"]["Value"].template get<int16_t>();
                        } else {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].physicalValue_isUsed = false;
                        }
                        if (inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j].contains("stringValue")) {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].stringValue_isUsed = true;
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].stringValue.charactersLen = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["stringValue"]["charactersLen"].template get<uint16_t>();
                            for (int k = 0; k < body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].stringValue.charactersLen; k++) {
                                body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].stringValue.characters[k] = inJson["Body"]["ServiceDetailRes"]["ServiceParameterList"]["ParameterSet"]["array"][i]["Parameter"]["array"][j]["stringValue"]["characters"][k].template get<char>();
                            }
                        } else {
                            body.ServiceDetailRes.ServiceParameterList.ParameterSet.array[i].Parameter.array[j].stringValue_isUsed = false;
                        }
                    }
                }
            } else {
                body.ServiceDetailRes.ServiceParameterList_isUsed = false;
            }
        } else if (packetType == "ServiceDiscoveryReq") {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_ServiceDiscoveryReqType(&body.ServiceDiscoveryReq);
            body.ServiceDiscoveryReq_isUsed = true;

            if (inJson["Body"]["ServiceDiscoveryReq"].contains("ServiceScope")) {
                body.ServiceDiscoveryReq.ServiceScope_isUsed = true;
                body.ServiceDiscoveryReq.ServiceScope.charactersLen = inJson["Body"]["ServiceDiscoveryReq"]["ServiceScope"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.ServiceDiscoveryReq.ServiceScope.charactersLen; i++) {
                    body.ServiceDiscoveryReq.ServiceScope.characters[i] = inJson["Body"]["ServiceDiscoveryReq"]["ServiceScope"]["characters"][i].template get<char>();
                }
            } else {
                body.ServiceDiscoveryReq.ServiceScope_isUsed = false;
            }

            if (inJson["Body"]["ServiceDiscoveryReq"].contains("ServiceCategory")) {
                body.ServiceDiscoveryReq.ServiceCategory_isUsed = true;
                body.ServiceDiscoveryReq.ServiceCategory = static_cast<iso2_serviceCategoryType>(inJson["Body"]["ServiceDiscoveryReq"]["ServiceCategory"].template get<int>());
            } else {
                body.ServiceDiscoveryReq.ServiceCategory_isUsed = false;
            }
        } else if (packetType == "ServiceDiscoveryRes") {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_ServiceDiscoveryResType(&body.ServiceDiscoveryRes);
            body.ServiceDiscoveryRes_isUsed = true;

            body.ServiceDiscoveryRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["ServiceDiscoveryRes"]["ResponseCode"].template get<int>());

            body.ServiceDiscoveryRes.PaymentOptionList.PaymentOption.arrayLen = inJson["Body"]["ServiceDiscoveryRes"]["PaymentOptionList"]["PaymentOption"]["arrayLen"].template get<uint16_t>();
            for (int i = 0; i < body.ServiceDiscoveryRes.PaymentOptionList.PaymentOption.arrayLen; i++) {
                body.ServiceDiscoveryRes.PaymentOptionList.PaymentOption.array[i] = static_cast<iso2_paymentOptionType>(inJson["Body"]["ServiceDiscoveryRes"]["PaymentOptionList"]["PaymentOption"]["array"][i].template get<int>());
            }

            body.ServiceDiscoveryRes.ChargeService.ServiceID = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceID"].template get<uint8_t>();

            if (inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"].contains("ServiceName")) {
                body.ServiceDiscoveryRes.ChargeService.ServiceName_isUsed = true;
                body.ServiceDiscoveryRes.ChargeService.ServiceName.charactersLen = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceName"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.ServiceDiscoveryRes.ChargeService.ServiceName.charactersLen; i++) {
                    body.ServiceDiscoveryRes.ChargeService.ServiceName.characters[i] = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceName"]["characters"][i].template get<char>();
                }
            } else {
                body.ServiceDiscoveryRes.ChargeService.ServiceName_isUsed = false;
            }

            body.ServiceDiscoveryRes.ChargeService.ServiceCategory = static_cast<iso2_serviceCategoryType>(inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceCategory"].template get<int>());

            if (inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"].contains("ServiceScope")) {
                body.ServiceDiscoveryRes.ChargeService.ServiceScope_isUsed = true;
                body.ServiceDiscoveryRes.ChargeService.ServiceScope.charactersLen = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceScope"]["charactersLen"].template get<uint16_t>();
                for (int i = 0; i < body.ServiceDiscoveryRes.ChargeService.ServiceScope.charactersLen; i++) {
                    body.ServiceDiscoveryRes.ChargeService.ServiceScope.characters[i] = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceScope"]["characters"][i].template get<char>();
                }
            } else {
                body.ServiceDiscoveryRes.ChargeService.ServiceScope_isUsed = false;
            }

            body.ServiceDiscoveryRes.ChargeService.FreeService = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["FreeService"].template get<bool>();

            body.ServiceDiscoveryRes.ChargeService.SupportedEnergyTransferMode.EnergyTransferMode.arrayLen = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["SupportedEnergyTransferMode"]["EnergyTransferMode"]["arrayLen"].template get<uint16_t>();
            for (int i = 0; i < body.ServiceDiscoveryRes.ChargeService.SupportedEnergyTransferMode.EnergyTransferMode.arrayLen; i++) {
                body.ServiceDiscoveryRes.ChargeService.SupportedEnergyTransferMode.EnergyTransferMode.array[i] = static_cast<iso2_EnergyTransferModeType>(inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["SupportedEnergyTransferMode"]["EnergyTransferMode"]["array"][i].template get<int>());
            }

            if (inJson["Body"]["ServiceDiscoveryRes"].contains("ServiceList")) {
                body.ServiceDiscoveryRes.ServiceList_isUsed = true;
                body.ServiceDiscoveryRes.ServiceList.Service.arrayLen = inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["arrayLen"].template get<uint16_t>();
                for (int i = 0; i < body.ServiceDiscoveryRes.ServiceList.Service.arrayLen; i++) {
                    body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceID = inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["ServiceID"].template get<uint8_t>();

                    if (inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i].contains("ServiceName")) {
                        body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceName_isUsed = true;
                        body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceName.charactersLen = inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["ServiceName"]["charactersLen"].template get<uint16_t>();
                        for (int j = 0; j < body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceName.charactersLen; j++) {
                            body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceName.characters[j] = inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["ServiceName"]["characters"][j].template get<char>();
                        }
                    } else {
                        body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceName_isUsed = false;
                    }

                    body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceCategory = static_cast<iso2_serviceCategoryType>(inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["ServiceCategory"].template get<int>());

                    if (inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i].contains("ServiceScope")) {
                        body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceScope_isUsed = true;
                        body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceScope.charactersLen = inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["ServiceScope"]["charactersLen"].template get<uint16_t>();
                        for (int j = 0; j < body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceScope.charactersLen; j++) {
                            body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceScope.characters[j] = inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["array"][i]["ServiceScope"]["characters"][j].template get<char>();
                        }
                    } else {
                        body.ServiceDiscoveryRes.ServiceList.Service.array[i].ServiceScope_isUsed = false;
                    }
                }
            } else {
                body.ServiceDiscoveryRes.ServiceList_isUsed = false;
            }
        } else if (packetType == "SessionSetupReq") {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_SessionSetupReqType(&body.SessionSetupReq);
            body.SessionSetupReq_isUsed = true;

            body.SessionSetupReq.EVCCID.bytesLen = inJson["Body"]["SessionSetupReq"]["EVCCID"]["bytesLen"].template get<uint8_t>();
            for (int i = 0; i < body.SessionSetupReq.EVCCID.bytesLen; i++) {
                body.SessionSetupReq.EVCCID.bytes[i] = inJson["Body"]["SessionSetupReq"]["EVCCID"]["bytes"][i].template get<uint8_t>();
            }
        } else if (packetType == "SessionSetupRes") {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_SessionSetupResType(&body.SessionSetupRes);
            body.SessionSetupRes_isUsed = true;

            body.SessionSetupRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["SessionSetupRes"]["ResponseCode"].template get<int>());

            body.SessionSetupRes.EVSEID.charactersLen = inJson["Body"]["SessionSetupRes"]["EVSEID"]["charactersLen"].template get<uint16_t>();
            for (int i = 0; i < body.SessionSetupRes.EVSEID.charactersLen; i++) {
                body.SessionSetupRes.EVSEID.characters[i] = inJson["Body"]["SessionSetupRes"]["EVSEID"]["characters"][i].template get<char>();
            }

            if (inJson["Body"]["SessionSetupRes"].contains("EVSETimeStamp")) {
                body.SessionSetupRes.EVSETimeStamp_isUsed = true;
                body.SessionSetupRes.EVSETimeStamp = inJson["Body"]["SessionSetupRes"]["EVSETimeStamp"].template get<uint64_t>();
            } else {
                body.SessionSetupRes.EVSETimeStamp_isUsed = false;
            }
        } else if (packetType == "SessionStopReq") {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_SessionStopReqType(&body.SessionStopReq);
            body.SessionStopReq_isUsed = true;

            body.SessionStopReq.ChargingSession = static_cast<iso2_chargingSessionType>(inJson["Body"]["SessionStopReq"]["ChargingSession"].template get<int>());
        } else if (packetType == "SessionStopRes") {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_SessionStopResType(&body.SessionStopRes);
            body.SessionStopRes_isUsed = true;

            body.SessionStopRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["SessionStopRes"]["ResponseCode"].template get<int>());
        } else if (packetType == "WeldingDetectionReq") {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_WeldingDetectionReqType(&body.WeldingDetectionReq);
            body.WeldingDetectionReq_isUsed = true;

            body.WeldingDetectionReq.DC_EVStatus.EVReady = inJson["Body"]["WeldingDetectionReq"]["DC_EVStatus"]["EVReady"].template get<bool>();
            body.WeldingDetectionReq.DC_EVStatus.EVErrorCode = static_cast<iso2_DC_EVErrorCodeType>(inJson["Body"]["WeldingDetectionReq"]["DC_EVStatus"]["EVErrorCode"].template get<int>());
            body.WeldingDetectionReq.DC_EVStatus.EVRESSSOC = inJson["Body"]["WeldingDetectionReq"]["DC_EVStatus"]["EVRESSSOC"].template get<uint8_t>();
        } else if (packetType == "WeldingDetectionRes") {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_iso2_WeldingDetectionResType(&body.WeldingDetectionRes);
            body.WeldingDetectionRes_isUsed = true;

            body.WeldingDetectionRes.ResponseCode = static_cast<iso2_responseCodeType>(inJson["Body"]["WeldingDetectionRes"]["ResponseCode"].template get<int>());

            body.WeldingDetectionRes.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["WeldingDetectionRes"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint16_t>();
            body.WeldingDetectionRes.DC_EVSEStatus.EVSENotification = static_cast<iso2_EVSENotificationType>(inJson["Body"]["WeldingDetectionRes"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());
            if (inJson["Body"]["WeldingDetectionRes"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                body.WeldingDetectionRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                body.WeldingDetectionRes.DC_EVSEStatus.EVSEIsolationStatus = static_cast<iso2_isolationLevelType>(inJson["Body"]["WeldingDetectionRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
            } else {
                body.WeldingDetectionRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
            }
            body.WeldingDetectionRes.DC_EVSEStatus.EVSEStatusCode = static_cast<iso2_DC_EVSEStatusCodeType>(inJson["Body"]["WeldingDetectionRes"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());

            body.WeldingDetectionRes.EVSEPresentVoltage.Multiplier = inJson["Body"]["WeldingDetectionRes"]["EVSEPresentVoltage"]["Multiplier"].template get<int8_t>();
            body.WeldingDetectionRes.EVSEPresentVoltage.Unit = static_cast<iso2_unitSymbolType>(inJson["Body"]["WeldingDetectionRes"]["EVSEPresentVoltage"]["Unit"].template get<int>());
            body.WeldingDetectionRes.EVSEPresentVoltage.Value = inJson["Body"]["WeldingDetectionRes"]["EVSEPresentVoltage"]["Value"].template get<int16_t>();
        } else {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        }

        uint8_t* stream = new uint8_t[256];
        exi_bitstream_t outEXI;
        size_t pos1 = 0;

        exi_bitstream_init(&outEXI, stream, 256, pos1, nullptr);
        encode_iso2_exiDocument(&outEXI, &inDoc);
        
        encoded_data* result = new encoded_data;
        result->size = exi_bitstream_get_length(&outEXI);
        result->buffer = new uint8_t[result->size];
        memcpy(result->buffer, stream, result->size);
        
        delete[] stream;
        return result;
    }

    void free_encoded_data(encoded_data* data) {
        if (data) {
            delete[] data->buffer;
            delete data;
        }
    }
}

int main(int argc, char* argv[]) {
    cxxopts::Options options("./ISO2Processor --[encode/decode]='STRING_OPTIONAL' -i FILE_INPUT -o FILE_OUTPUT\n  ./ISO2Processor --decode='809a021050908c0c0c0c0c51e000040080'\n  ./ISO2Processor -e -i input.json -o output.txt\n  ./ISO2Processor", "Encode and decode ISO2 EXI messages\nIf no arguement specified for input/outfile, will default to stdin/stdout\nMUST use double quotes (\") and lower case true/false");
    options.add_options()
        ("h,help", "Show help")
        ("e,encode", "Encode EXI message from JSON to EXI bytes", cxxopts::value<std::string>()->implicit_value("cmd"))
        ("d,decode", "Decode EXI message from EXI bytes to JSON", cxxopts::value<std::string>()->implicit_value("cmd"))
        ("i,input", "Input file to read JSON or EXI bytes from", cxxopts::value<std::string>())
        ("o,output", "Output file to write JSON or EXI bytes to", cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    // Error handling
    if (result.count("encode") == 0 && result.count("decode") == 0) {
        std::cerr << "Error: Must specify either encode or decode option." << std::endl;
        std::cout << options.help() << std::endl;
        return 1;
    }

    if (result.count("encode") && result.count("decode")) {
        std::cerr << "Error: Cannot specify both encode and decode options." << std::endl;
        std::cout << options.help() << std::endl;
        return 1;
    }

    if (result.count("encode")) {
        if (result["encode"].as<std::string>() != "cmd" && result.count("input")) {
            std::cerr << "Error: Cannot encode string input with file input option." << std::endl;
            std::cout << options.help() << std::endl;
            return 1;
        }

        string inString;
        if (result.count("input")) {
            ifstream inputFile(result["input"].as<std::string>());
            if (inputFile.is_open()) {
                stringstream buffer;
                buffer << inputFile.rdbuf();
                inString = buffer.str();
                inputFile.close();
            } else {
                std::cerr << "Error: Could not open input file." << std::endl;
                return 1;
            }
        
        } else {
            inString = result["encode"].as<std::string>();
        }

        // cout << "Input JSON string: " << inString << endl;

        bool isValid = json::accept(inString);
        if (!isValid) {
            std::cerr << "Error: Invalid JSON input." << std::endl;
            return 1;
        }

        encoded_data* encodedResult = encodeFromPythonDict(inString.c_str());
        if (result.count("output")) {
            ofstream outputFile(result["output"].as<std::string>());
            if (outputFile.is_open()) {
                // Convert bytes to hex string
                stringstream hexStream;
                for (size_t i = 0; i < encodedResult->size; ++i) {
                    hexStream << std::hex << std::setw(2) << std::setfill('0') << (int)encodedResult->buffer[i];
                }
                outputFile << hexStream.str();
                outputFile.close();
            } else {
                std::cerr << "Error: Could not open output file." << std::endl;
                free_encoded_data(encodedResult);
                return 1;
            }
        } else {
            for (size_t i = 0; i < encodedResult->size; ++i) {
                cout << std::hex << std::setw(2) << std::setfill('0') << (int)encodedResult->buffer[i];
            }
            cout << endl;
        }

    }

    if (result.count("decode")) {
        if (result["decode"].as<std::string>() != "cmd" && result.count("input")) {
            std::cerr << "Error: Cannot decode string input with file input option." << std::endl;
            std::cout << options.help() << std::endl;
            return 1;
        }

        string hexString;
        if (result.count("input")) {
            ifstream inputFile(result["input"].as<std::string>());
            if (inputFile.is_open()) {
                stringstream buffer;
                buffer << inputFile.rdbuf();
                hexString = buffer.str();
                inputFile.close();
            } else {
                std::cerr << "Error: Could not open input file." << std::endl;
                return 1;
            }
        } else {
            hexString = result["decode"].as<std::string>();
        }

        // Convert hex string to bytes
        if (hexString.length() % 2 != 0) {
            std::cerr << "Error: Invalid hex string length." << std::endl;
            return 1;
        }

        vector<uint8_t> bytes;
        for (size_t i = 0; i < hexString.length(); i += 2) {
            string byteString = hexString.substr(i, 2);
            try {
                uint8_t byte = static_cast<uint8_t>(stoi(byteString, nullptr, 16));
                bytes.push_back(byte);
            } catch (const std::exception& e) {
                std::cerr << "Error: Invalid hex string format." << std::endl;
                return 1;
            }
        }

        const char* jsonString = decodeFromPythonBytes(bytes.data(), bytes.size());
        if (result.count("output")) {
            ofstream outputFile(result["output"].as<std::string>());
            if (outputFile.is_open()) {
                // Try to parse and pretty print JSON
                try {
                    json j = json::parse(jsonString);
                    outputFile << j.dump(4);
                    outputFile.close();
                } catch (const json::parse_error& e) {
                    std::cerr << "Error: Failed to parse JSON output." << std::endl;
                    return 1;
                }
            } else {
                std::cerr << "Error: Could not open output file." << std::endl;
                return 1;
            }
        } else {
            try {
                json j = json::parse(jsonString);
                cout << j.dump(4) << endl;
            } catch (const json::parse_error& e) {
                std::cerr << "Error: Failed to parse JSON output." << std::endl;
                return 1;
            }
        }
    }
    return 0;
}