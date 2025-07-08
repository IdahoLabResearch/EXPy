/*
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
*/

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <new>

#include <cbv2g/din/din_msgDefDecoder.h>
#include <cbv2g/din/din_msgDefEncoder.h>
#include <cbv2g/din/din_msgDefDatatypes.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <cxxopts.hpp>

#include <iostream>
#include <fstream>
using namespace std;

extern "C" {
    const char* decodeFromPythonBytes(const uint8_t* data, size_t size) {
        din_exiDocument outDoc;
        exi_bitstream_t inEXI;

        json outJson;

        exi_bitstream_init(&inEXI, const_cast<uint8_t*>(data), size, 0, nullptr);
        decode_din_exiDocument(&inEXI, &outDoc);
        // int res = decode_din_exiDocument(&exiIn, &docOut);
        // cout << "Decoding EXI String returned with value of: " << res << endl;

        const auto& header = outDoc.V2G_Message.Header;
        const auto& body = outDoc.V2G_Message.Body;

        // const auto sessionID = vector<uint8_t>(begin(header.SessionID.bytes), end(header.SessionID.bytes));
        // outJson["Header"]["SessionID"] = sessionID;

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

        if (header.Signature_isUsed) {
            if (header.Signature.Id_isUsed) {
                outJson["Header"]["Signature"]["Id"]["charactersLen"] = header.Signature.Id.charactersLen;
                for (int i = 0; i < header.Signature.Id.charactersLen; i++) {
                    outJson["Header"]["Signature"]["Id"][i] = header.Signature.Id.characters[i];
                }
            }
            if (header.Signature.SignedInfo.Id_isUsed) {
                outJson["Header"]["Signature"]["SignedInfo"]["Id"]["charactersLen"] = header.Signature.SignedInfo.Id.charactersLen;
                for (int i = 0; i < header.Signature.SignedInfo.Id.charactersLen; i++) {
                    outJson["Header"]["Signature"]["SignedInfo"]["Id"][i] = header.Signature.SignedInfo.Id.characters[i];
                }
            }
            outJson["Header"]["Signature"]["SignedInfo"]["CanonicalizationMethod"]["Algorithm"]["charactersLen"] = header.Signature.SignedInfo.CanonicalizationMethod.Algorithm.charactersLen;
            for (int i = 0; i < header.Signature.SignedInfo.CanonicalizationMethod.Algorithm.charactersLen; i++) {
                outJson["Header"]["Signature"]["SignedInfo"]["CanonicalizationMethod"]["Algorithm"][i] = header.Signature.SignedInfo.CanonicalizationMethod.Algorithm.characters[i];
            }
            if (header.Signature.SignedInfo.CanonicalizationMethod.ANY_isUsed) {
                outJson["Header"]["Signature"]["SignedInfo"]["CanonicalizationMethod"]["ANY"]["bytesLen"] = header.Signature.SignedInfo.CanonicalizationMethod.ANY.bytesLen;
                for (int i = 0; i < header.Signature.SignedInfo.CanonicalizationMethod.ANY.bytesLen; i++) {
                    outJson["Header"]["Signature"]["SignedInfo"]["CanonicalizationMethod"]["ANY"][i] = header.Signature.SignedInfo.CanonicalizationMethod.ANY.bytes[i];
                }
            }
            outJson["Header"]["Signature"]["SignedInfo"]["SignatureMethod"]["Algorithm"]["charactersLen"] = header.Signature.SignedInfo.SignatureMethod.Algorithm.charactersLen;
            for (int i = 0; i < header.Signature.SignedInfo.SignatureMethod.Algorithm.charactersLen; i++) {
                outJson["Header"]["Signature"]["SignedInfo"]["SignatureMethod"]["Algorithm"][i] = header.Signature.SignedInfo.SignatureMethod.Algorithm.characters[i];
            }
            if (header.Signature.SignedInfo.SignatureMethod.HMACOutputLength_isUsed) {
                outJson["Header"]["Signature"]["SignedInfo"]["SignatureMethod"]["HMACOutputLength"]["data"]["octets_count"] = header.Signature.SignedInfo.SignatureMethod.HMACOutputLength.data.octets_count;
                for (size_t i = 0; i < header.Signature.SignedInfo.SignatureMethod.HMACOutputLength.data.octets_count; i++) {
                    outJson["Header"]["Signature"]["SignedInfo"]["SignatureMethod"]["HMACOutputLength"]["data"][i] = header.Signature.SignedInfo.SignatureMethod.HMACOutputLength.data.octets[i];
                }
                outJson["Header"]["Signature"]["SignedInfo"]["SignatureMethod"]["HMACOutputLength"]["is_negative"] = header.Signature.SignedInfo.SignatureMethod.HMACOutputLength.is_negative;
            }
            if (header.Signature.SignedInfo.SignatureMethod.ANY_isUsed) {
                outJson["Header"]["Signature"]["SignedInfo"]["SignatureMethod"]["ANY"]["bytesLen"] = header.Signature.SignedInfo.SignatureMethod.ANY.bytesLen;
                for (int i = 0; i < header.Signature.SignedInfo.SignatureMethod.ANY.bytesLen; i++) {
                    outJson["Header"]["Signature"]["SignedInfo"]["SignatureMethod"]["ANY"][i] = header.Signature.SignedInfo.SignatureMethod.ANY.bytes[i];
                }
            }
            if (header.Signature.SignedInfo.Reference.Id_isUsed) {
                outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["Id"]["charactersLen"] = header.Signature.SignedInfo.Reference.Id.charactersLen;
                for (int i = 0; i < header.Signature.SignedInfo.Reference.Id.charactersLen; i++) {
                    outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["Id"][i] = header.Signature.SignedInfo.Reference.Id.characters[i];
                }
            }
            if (header.Signature.SignedInfo.Reference.Type_isUsed) {
                outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["Type"]["charactersLen"] = header.Signature.SignedInfo.Reference.Type.charactersLen;
                for (int i = 0; i < header.Signature.SignedInfo.Reference.Type.charactersLen; i++) {
                    outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["Type"][i] = header.Signature.SignedInfo.Reference.Type.characters[i];
                }
            }
            if (header.Signature.SignedInfo.Reference.URI_isUsed) {
                outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["URI"]["charactersLen"] = header.Signature.SignedInfo.Reference.URI.charactersLen;
                for (int i = 0; i < header.Signature.SignedInfo.Reference.URI.charactersLen; i++) {
                    outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["URI"][i] = header.Signature.SignedInfo.Reference.URI.characters[i];
                }
            }
            if (header.Signature.SignedInfo.Reference.Transforms_isUsed) {
                outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["Transforms"]["Transform"]["Algorithm"]["charactersLen"] = header.Signature.SignedInfo.Reference.Transforms.Transform.Algorithm.charactersLen;
                for (int i = 0; i < header.Signature.SignedInfo.Reference.Transforms.Transform.Algorithm.charactersLen; i++) {
                    outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["Transforms"]["Transform"]["Algorithm"][i] = header.Signature.SignedInfo.Reference.Transforms.Transform.Algorithm.characters[i];
                }
                if (header.Signature.SignedInfo.Reference.Transforms.Transform.ANY_isUsed) {
                    outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["Transforms"]["Transform"]["ANY"]["bytesLen"] = header.Signature.SignedInfo.Reference.Transforms.Transform.ANY.bytesLen;
                    for (int i = 0; i < header.Signature.SignedInfo.Reference.Transforms.Transform.ANY.bytesLen; i++) {
                        outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["Transforms"]["Transform"]["ANY"][i] = header.Signature.SignedInfo.Reference.Transforms.Transform.ANY.bytes[i];
                    }
                }
                if (header.Signature.SignedInfo.Reference.Transforms.Transform.XPath_isUsed) {
                    outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["Transforms"]["Transform"]["XPath"]["charactersLen"] = header.Signature.SignedInfo.Reference.Transforms.Transform.XPath.charactersLen;
                    for (int i = 0; i < header.Signature.SignedInfo.Reference.Transforms.Transform.XPath.charactersLen; i++) {
                        outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["Transforms"]["Transform"]["XPath"][i] = header.Signature.SignedInfo.Reference.Transforms.Transform.XPath.characters[i];
                    }
                }
            }
            outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["DigestMethod"]["Algorithm"]["charactersLen"] = header.Signature.SignedInfo.Reference.DigestMethod.Algorithm.charactersLen;
            for (int i = 0; i < header.Signature.SignedInfo.Reference.DigestMethod.Algorithm.charactersLen; i++) {
                outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["DigestMethod"]["Algorithm"][i] = header.Signature.SignedInfo.Reference.DigestMethod.Algorithm.characters[i];
            }
            if (header.Signature.SignedInfo.Reference.DigestMethod.ANY_isUsed) {
                outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["DigestMethod"]["ANY"]["bytesLen"] = header.Signature.SignedInfo.Reference.DigestMethod.ANY.bytesLen;
                for (int i = 0; i < header.Signature.SignedInfo.Reference.DigestMethod.ANY.bytesLen; i++) {
                    outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["DigestMethod"]["ANY"][i] = header.Signature.SignedInfo.Reference.DigestMethod.ANY.bytes[i];
                }
            }
            outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["DigestValue"]["bytesLen"] = header.Signature.SignedInfo.Reference.DigestValue.bytesLen;
            for (int i = 0; i < header.Signature.SignedInfo.Reference.DigestValue.bytesLen; i++) {
                outJson["Header"]["Signature"]["SignedInfo"]["Reference"]["DigestValue"][i] = header.Signature.SignedInfo.Reference.DigestValue.bytes[i];
            }
            
            if (header.Signature.SignatureValue.Id_isUsed) {
                outJson["Header"]["Signature"]["SignatureValue"]["Id"]["charactersLen"] = header.Signature.SignatureValue.Id.charactersLen;
                for (int i = 0; i < header.Signature.SignatureValue.Id.charactersLen; i++) {
                    outJson["Header"]["Signature"]["SignatureValue"]["Id"][i] = header.Signature.SignatureValue.Id.characters[i];
                }
            }
            outJson["Header"]["Signature"]["SignatureValue"]["CONTENT"]["bytesLen"] = header.Signature.SignatureValue.CONTENT.bytesLen;
            for (int i = 0; i < header.Signature.SignatureValue.CONTENT.bytesLen; i++) {
                outJson["Header"]["Signature"]["SignatureValue"]["CONTENT"][i] = header.Signature.SignatureValue.CONTENT.bytes[i];
            }

            if (header.Signature.KeyInfo_isUsed) {
                if (header.Signature.KeyInfo.Id_isUsed) {
                    outJson["Header"]["Signature"]["KeyInfo"]["Id"]["charactersLen"] = header.Signature.KeyInfo.Id.charactersLen;
                    for (int i = 0; i < header.Signature.KeyInfo.Id.charactersLen; i++) {
                        outJson["Header"]["Signature"]["KeyInfo"]["Id"][i] = header.Signature.KeyInfo.Id.characters[i];
                    }
                }
                if (header.Signature.KeyInfo.KeyName_isUsed) {
                    outJson["Header"]["Signature"]["KeyInfo"]["KeyName"]["charactersLen"] = header.Signature.KeyInfo.KeyName.charactersLen;
                    for (int i = 0; i < header.Signature.KeyInfo.KeyName.charactersLen; i++) {
                        outJson["Header"]["Signature"]["KeyInfo"]["KeyName"][i] = header.Signature.KeyInfo.KeyName.characters[i];
                    }
                }
                if (header.Signature.KeyInfo.KeyValue_isUsed) {
                    if (header.Signature.KeyInfo.KeyValue.DSAKeyValue_isUsed) {
                        if (header.Signature.KeyInfo.KeyValue.DSAKeyValue.P_isUsed) {
                            outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["P"]["bytesLen"] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.P.bytesLen;
                            for (int i = 0; i < header.Signature.KeyInfo.KeyValue.DSAKeyValue.P.bytesLen; i++) {
                                outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["P"][i] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.P.bytes[i];
                            }
                        }
                        if (header.Signature.KeyInfo.KeyValue.DSAKeyValue.Q_isUsed) {
                            outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["Q"]["bytesLen"] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.Q.bytesLen;
                            for (int i = 0; i < header.Signature.KeyInfo.KeyValue.DSAKeyValue.Q.bytesLen; i++) {
                                outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["Q"][i] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.Q.bytes[i];
                            }
                        }
                        if (header.Signature.KeyInfo.KeyValue.DSAKeyValue.G_isUsed) {
                            outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["G"]["bytesLen"] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.G.bytesLen;
                            for (int i = 0; i < header.Signature.KeyInfo.KeyValue.DSAKeyValue.G.bytesLen; i++) {
                                outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["G"][i] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.G.bytes[i];
                            }
                        }

                        outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["Y"]["bytesLen"] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.Y.bytesLen;
                        for (int i = 0; i < header.Signature.KeyInfo.KeyValue.DSAKeyValue.Y.bytesLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["Y"][i] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.Y.bytes[i];
                        }
                        
                        if (header.Signature.KeyInfo.KeyValue.DSAKeyValue.J_isUsed) {
                            outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["J"]["bytesLen"] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.J.bytesLen;
                            for (int i = 0; i < header.Signature.KeyInfo.KeyValue.DSAKeyValue.J.bytesLen; i++) {
                                outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["J"][i] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.J.bytes[i];
                            }
                        }
                        if (header.Signature.KeyInfo.KeyValue.DSAKeyValue.Seed_isUsed) {
                            outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["Seed"]["bytesLen"] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.Seed.bytesLen;
                            for (int i = 0; i < header.Signature.KeyInfo.KeyValue.DSAKeyValue.Seed.bytesLen; i++) {
                                outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["Seed"][i] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.Seed.bytes[i];
                            }
                        }
                        if (header.Signature.KeyInfo.KeyValue.DSAKeyValue.PgenCounter_isUsed) {
                            outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["PgenCounter"]["bytesLen"] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.PgenCounter.bytesLen;
                            for (int i = 0; i < header.Signature.KeyInfo.KeyValue.DSAKeyValue.PgenCounter.bytesLen; i++) {
                                outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["DSAKeyValue"]["PgenCounter"][i] = header.Signature.KeyInfo.KeyValue.DSAKeyValue.PgenCounter.bytes[i];
                            }
                        }
                    }
                    if (header.Signature.KeyInfo.KeyValue.RSAKeyValue_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["RSAKeyValue"]["Modulus"]["bytesLen"] = header.Signature.KeyInfo.KeyValue.RSAKeyValue.Modulus.bytesLen;
                        for (int i = 0; i < header.Signature.KeyInfo.KeyValue.RSAKeyValue.Modulus.bytesLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["RSAKeyValue"]["Modulus"][i] = header.Signature.KeyInfo.KeyValue.RSAKeyValue.Modulus.bytes[i];
                        }
                        outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["RSAKeyValue"]["Exponent"]["bytesLen"] = header.Signature.KeyInfo.KeyValue.RSAKeyValue.Exponent.bytesLen;
                        for (int i = 0; i < header.Signature.KeyInfo.KeyValue.RSAKeyValue.Exponent.bytesLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["RSAKeyValue"]["Exponent"][i] = header.Signature.KeyInfo.KeyValue.RSAKeyValue.Exponent.bytes[i];
                        }
                    }
                    if (header.Signature.KeyInfo.KeyValue.ANY_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["ANY"]["bytesLen"] = header.Signature.KeyInfo.KeyValue.ANY.bytesLen;
                        for (int i = 0; i < header.Signature.KeyInfo.KeyValue.ANY.bytesLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["KeyValue"]["ANY"][i] = header.Signature.KeyInfo.KeyValue.ANY.bytes[i];
                        }
                    }
                }

                if (header.Signature.KeyInfo.RetrievalMethod_isUsed) {
                    if (header.Signature.KeyInfo.RetrievalMethod.Type_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["RetrievalMethod"]["Type"]["charactersLen"] = header.Signature.KeyInfo.RetrievalMethod.Type.charactersLen;
                        for (int i = 0; i < header.Signature.KeyInfo.RetrievalMethod.Type.charactersLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["RetrievalMethod"]["Type"][i] = header.Signature.KeyInfo.RetrievalMethod.Type.characters[i];
                        }
                    }
                    if (header.Signature.KeyInfo.RetrievalMethod.URI_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["RetrievalMethod"]["URI"]["charactersLen"] = header.Signature.KeyInfo.RetrievalMethod.URI.charactersLen;
                        for (int i = 0; i < header.Signature.KeyInfo.RetrievalMethod.URI.charactersLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["RetrievalMethod"]["URI"][i] = header.Signature.KeyInfo.RetrievalMethod.URI.characters[i];
                        }
                    }
                    if (header.Signature.KeyInfo.RetrievalMethod.Transforms_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["RetrievalMethod"]["Transforms"]["Transform"]["Algorithm"]["charactersLen"] = header.Signature.KeyInfo.RetrievalMethod.Transforms.Transform.Algorithm.charactersLen;
                        for (int i = 0; i < header.Signature.KeyInfo.RetrievalMethod.Transforms.Transform.Algorithm.charactersLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["RetrievalMethod"]["Transforms"]["Transform"]["Algorithm"][i] = header.Signature.KeyInfo.RetrievalMethod.Transforms.Transform.Algorithm.characters[i];
                        }
                        if (header.Signature.KeyInfo.RetrievalMethod.Transforms.Transform.ANY_isUsed) {
                            outJson["Header"]["Signature"]["KeyInfo"]["RetrievalMethod"]["Transforms"]["Transform"]["ANY"]["bytesLen"] = header.Signature.KeyInfo.RetrievalMethod.Transforms.Transform.ANY.bytesLen;
                            for (int i = 0; i < header.Signature.KeyInfo.RetrievalMethod.Transforms.Transform.ANY.bytesLen; i++) {
                                outJson["Header"]["Signature"]["KeyInfo"]["RetrievalMethod"]["Transforms"]["Transform"]["ANY"][i] = header.Signature.KeyInfo.RetrievalMethod.Transforms.Transform.ANY.bytes[i];
                            }
                        }
                        if (header.Signature.KeyInfo.RetrievalMethod.Transforms.Transform.XPath_isUsed) {
                            outJson["Header"]["Signature"]["KeyInfo"]["RetrievalMethod"]["Transforms"]["Transform"]["XPath"]["charactersLen"] = header.Signature.KeyInfo.RetrievalMethod.Transforms.Transform.XPath.charactersLen;
                            for (int i = 0; i < header.Signature.KeyInfo.RetrievalMethod.Transforms.Transform.XPath.charactersLen; i++) {
                                outJson["Header"]["Signature"]["KeyInfo"]["RetrievalMethod"]["Transforms"]["Transform"]["XPath"][i] = header.Signature.KeyInfo.RetrievalMethod.Transforms.Transform.XPath.characters[i];
                            }
                        }
                    }
                }
                
                if (header.Signature.KeyInfo.X509Data_isUsed) {
                    if (header.Signature.KeyInfo.X509Data.X509IssuerSerial_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509IssuerSerial"]["X509IssuerName"]["charactersLen"] = header.Signature.KeyInfo.X509Data.X509IssuerSerial.X509IssuerName.charactersLen;
                        for (int i = 0; i < header.Signature.KeyInfo.X509Data.X509IssuerSerial.X509IssuerName.charactersLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509IssuerSerial"]["X509IssuerName"][i] = header.Signature.KeyInfo.X509Data.X509IssuerSerial.X509IssuerName.characters[i];
                        }
                        outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509IssuerSerial"]["X509SerialNumber"]["data"]["octets_count"] = header.Signature.KeyInfo.X509Data.X509IssuerSerial.X509SerialNumber.data.octets_count;
                        for (size_t i = 0; i < header.Signature.KeyInfo.X509Data.X509IssuerSerial.X509SerialNumber.data.octets_count; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509IssuerSerial"]["X509SerialNumber"]["data"][i] = header.Signature.KeyInfo.X509Data.X509IssuerSerial.X509SerialNumber.data.octets[i];
                        }
                        outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509IssuerSerial"]["X509SerialNumber"]["is_negative"] = header.Signature.KeyInfo.X509Data.X509IssuerSerial.X509SerialNumber.is_negative;
                    }
                    if (header.Signature.KeyInfo.X509Data.X509SKI_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509SKI"]["bytesLen"] = header.Signature.KeyInfo.X509Data.X509SKI.bytesLen;
                        for (int i = 0; i < header.Signature.KeyInfo.X509Data.X509SKI.bytesLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509SKI"][i] = header.Signature.KeyInfo.X509Data.X509SKI.bytes[i];
                        }
                    }
                    if (header.Signature.KeyInfo.X509Data.X509SubjectName_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509SubjectName"]["charactersLen"] = header.Signature.KeyInfo.X509Data.X509SubjectName.charactersLen;
                        for (int i = 0; i < header.Signature.KeyInfo.X509Data.X509SubjectName.charactersLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509SubjectName"][i] = header.Signature.KeyInfo.X509Data.X509SubjectName.characters[i];
                        }
                    }
                    if (header.Signature.KeyInfo.X509Data.X509Certificate_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509Certificate"]["bytesLen"] = header.Signature.KeyInfo.X509Data.X509Certificate.bytesLen;
                        for (int i = 0; i < header.Signature.KeyInfo.X509Data.X509Certificate.bytesLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509Certificate"][i] = header.Signature.KeyInfo.X509Data.X509Certificate.bytes[i];
                        }
                    }
                    if (header.Signature.KeyInfo.X509Data.X509CRL_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509CRL"]["bytesLen"] = header.Signature.KeyInfo.X509Data.X509CRL.bytesLen;
                        for (int i = 0; i < header.Signature.KeyInfo.X509Data.X509CRL.bytesLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["X509CRL"][i] = header.Signature.KeyInfo.X509Data.X509CRL.bytes[i];
                        }
                    }
                    if (header.Signature.KeyInfo.X509Data.ANY_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["ANY"]["bytesLen"] = header.Signature.KeyInfo.X509Data.ANY.bytesLen;
                        for (int i = 0; i < header.Signature.KeyInfo.X509Data.ANY.bytesLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["X509Data"]["ANY"][i] = header.Signature.KeyInfo.X509Data.ANY.bytes[i];
                        }
                    }
                }
                if (header.Signature.KeyInfo.PGPData_isUsed) {
                    if (header.Signature.KeyInfo.PGPData.choice_1_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["PGPData"]["choice_1"]["PGPKeyID"]["bytesLen"] = header.Signature.KeyInfo.PGPData.choice_1.PGPKeyID.bytesLen;
                        for (int i = 0; i < header.Signature.KeyInfo.PGPData.choice_1.PGPKeyID.bytesLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["PGPData"]["choice_1"]["PGPKeyID"][i] = header.Signature.KeyInfo.PGPData.choice_1.PGPKeyID.bytes[i];
                        }
                        if (header.Signature.KeyInfo.PGPData.choice_1.PGPKeyPacket_isUsed) {
                            outJson["Header"]["Signature"]["KeyInfo"]["PGPData"]["choice_1"]["PGPKeyPacket"]["bytesLen"] = header.Signature.KeyInfo.PGPData.choice_1.PGPKeyPacket.bytesLen;
                            for (int i = 0; i < header.Signature.KeyInfo.PGPData.choice_1.PGPKeyPacket.bytesLen; i++) {
                                outJson["Header"]["Signature"]["KeyInfo"]["PGPData"]["choice_1"]["PGPKeyPacket"][i] = header.Signature.KeyInfo.PGPData.choice_1.PGPKeyPacket.bytes[i];
                            }
                        }
                        if (header.Signature.KeyInfo.PGPData.choice_1.ANY_isUsed) {
                            outJson["Header"]["Signature"]["KeyInfo"]["PGPData"]["choice_1"]["ANY"]["bytesLen"] = header.Signature.KeyInfo.PGPData.choice_1.ANY.bytesLen;
                            for (int i = 0; i < header.Signature.KeyInfo.PGPData.choice_1.ANY.bytesLen; i++) {
                                outJson["Header"]["Signature"]["KeyInfo"]["PGPData"]["choice_1"]["ANY"][i] = header.Signature.KeyInfo.PGPData.choice_1.ANY.bytes[i];
                            }
                        }
                    }
                    if (header.Signature.KeyInfo.PGPData.choice_2_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["PGPData"]["choice_2"]["PGPKeyPacket"]["bytesLen"] = header.Signature.KeyInfo.PGPData.choice_2.PGPKeyPacket.bytesLen;
                        for (int i = 0; i < header.Signature.KeyInfo.PGPData.choice_2.PGPKeyPacket.bytesLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["PGPData"]["choice_2"]["PGPKeyPacket"][i] = header.Signature.KeyInfo.PGPData.choice_2.PGPKeyPacket.bytes[i];
                        }
                        if (header.Signature.KeyInfo.PGPData.choice_2.ANY_isUsed) {
                            outJson["Header"]["Signature"]["KeyInfo"]["PGPData"]["choice_2"]["ANY"]["bytesLen"] = header.Signature.KeyInfo.PGPData.choice_2.ANY.bytesLen;
                            for (int i = 0; i < header.Signature.KeyInfo.PGPData.choice_2.ANY.bytesLen; i++) {
                                outJson["Header"]["Signature"]["KeyInfo"]["PGPData"]["choice_2"]["ANY"][i] = header.Signature.KeyInfo.PGPData.choice_2.ANY.bytes[i];
                            }
                        }
                    }
                }
                if (header.Signature.KeyInfo.SPKIData_isUsed) {
                    outJson["Header"]["Signature"]["KeyInfo"]["SPKIData"]["SPKISexp"]["bytesLen"] = header.Signature.KeyInfo.SPKIData.SPKISexp.bytesLen;
                    for (int i = 0; i < header.Signature.KeyInfo.SPKIData.SPKISexp.bytesLen; i++) {
                        outJson["Header"]["Signature"]["KeyInfo"]["SPKIData"]["SPKISexp"][i] = header.Signature.KeyInfo.SPKIData.SPKISexp.bytes[i];
                    }
                    if (header.Signature.KeyInfo.SPKIData.ANY_isUsed) {
                        outJson["Header"]["Signature"]["KeyInfo"]["SPKIData"]["ANY"]["bytesLen"] = header.Signature.KeyInfo.SPKIData.ANY.bytesLen;
                        for (int i = 0; i < header.Signature.KeyInfo.SPKIData.ANY.bytesLen; i++) {
                            outJson["Header"]["Signature"]["KeyInfo"]["SPKIData"]["ANY"][i] = header.Signature.KeyInfo.SPKIData.ANY.bytes[i];
                        }
                    }
                }
                if (header.Signature.KeyInfo.MgmtData_isUsed) {
                    outJson["Header"]["Signature"]["KeyInfo"]["MgmtData"]["charactersLen"] = header.Signature.KeyInfo.MgmtData.charactersLen;
                    for (int i = 0; i < header.Signature.KeyInfo.MgmtData.charactersLen; i++) {
                        outJson["Header"]["Signature"]["KeyInfo"]["MgmtData"][i] = header.Signature.KeyInfo.MgmtData.characters[i];
                    }
                }
                if (header.Signature.KeyInfo.ANY_isUsed) {
                    outJson["Header"]["Signature"]["KeyInfo"]["ANY"]["bytesLen"] = header.Signature.KeyInfo.ANY.bytesLen;
                    for (int i = 0; i < header.Signature.KeyInfo.ANY.bytesLen; i++) {
                        outJson["Header"]["Signature"]["KeyInfo"]["ANY"][i] = header.Signature.KeyInfo.ANY.bytes[i];
                    }
                }
            }
            if (header.Signature.Object_isUsed) {
                if (header.Signature.Object.Encoding_isUsed) {
                    outJson["Header"]["Signature"]["Object"]["Encoding"]["charactersLen"] = header.Signature.Object.Encoding.charactersLen;
                    for (int i = 0; i < header.Signature.Object.Encoding.charactersLen; i++) {
                        outJson["Header"]["Signature"]["Object"]["Encoding"][i] = header.Signature.Object.Encoding.characters[i];
                    }
                }
                if (header.Signature.Object.Id_isUsed) {
                    outJson["Header"]["Signature"]["Object"]["Id"]["charactersLen"] = header.Signature.Object.Id.charactersLen;
                    for (int i = 0; i < header.Signature.Object.Id.charactersLen; i++) {
                        outJson["Header"]["Signature"]["Object"]["Id"][i] = header.Signature.Object.Id.characters[i];
                    }
                }
                if (header.Signature.Object.MimeType_isUsed) {
                    outJson["Header"]["Signature"]["Object"]["MimeType"]["charactersLen"] = header.Signature.Object.MimeType.charactersLen;
                    for (int i = 0; i < header.Signature.Object.MimeType.charactersLen; i++) {
                        outJson["Header"]["Signature"]["Object"]["MimeType"][i] = header.Signature.Object.MimeType.characters[i];
                    }
                }
                if (header.Signature.Object.ANY_isUsed) {
                    outJson["Header"]["Signature"]["Object"]["ANY"]["bytesLen"] = header.Signature.Object.ANY.bytesLen;
                    for (int i = 0; i < header.Signature.Object.ANY.bytesLen; i++) {
                        outJson["Header"]["Signature"]["Object"]["ANY"][i] = header.Signature.Object.ANY.bytes[i];
                    }
                }
            }
        }

        if (body.CableCheckReq_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["CableCheckReq"]["isUsed"] = true;
            outJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVReady"] = body.CableCheckReq.DC_EVStatus.EVReady;

            if (body.CableCheckReq.DC_EVStatus.EVCabinConditioning_isUsed) {
                outJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVCabinConditioning"] = body.CableCheckReq.DC_EVStatus.EVCabinConditioning;
            }

            if (body.CableCheckReq.DC_EVStatus.EVRESSConditioning_isUsed) {
                outJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVRESSConditioning"] = body.CableCheckReq.DC_EVStatus.EVRESSConditioning;
            }

            outJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVErrorCode"] = body.CableCheckReq.DC_EVStatus.EVErrorCode;
            outJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVRESSSOC"] = body.CableCheckReq.DC_EVStatus.EVRESSSOC;
        } else if (body.CableCheckRes_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["CableCheckRes"]["isUsed"] = true;
            outJson["Body"]["CableCheckRes"]["ResponseCode"] = body.CableCheckRes.ResponseCode;

            if (body.CableCheckRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed) {
                outJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.CableCheckRes.DC_EVSEStatus.EVSEIsolationStatus;
            }

            outJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.CableCheckRes.DC_EVSEStatus.EVSEStatusCode;
            outJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.CableCheckRes.DC_EVSEStatus.NotificationMaxDelay;
            outJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["EVSENotification"] = body.CableCheckRes.DC_EVSEStatus.EVSENotification;
        
            outJson["Body"]["CableCheckRes"]["EVSEProcessing"] = body.CableCheckRes.EVSEProcessing;
        } else if (body.CertificateInstallationReq_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.CertificateInstallationRes_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.CertificateUpdateReq_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.CertificateUpdateRes_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.ChargeParameterDiscoveryReq_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["ChargeParameterDiscoveryReq"]["isUsed"] = true;

            outJson["Body"]["ChargeParameterDiscoveryReq"]["EVRequestedEnergyTransferType"] = body.ChargeParameterDiscoveryReq.EVRequestedEnergyTransferType;

            if (body.ChargeParameterDiscoveryReq.AC_EVChargeParameter_isUsed) {
                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["DepartureTime"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.DepartureTime;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxVoltage"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxVoltage.Value;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxCurrent"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxCurrent.Value;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMinCurrent"] = body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMinCurrent.Value;
            }

            if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter_isUsed) {
                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVReady"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVReady;
                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVCabinConditioning_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVCabinConditioning"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVCabinConditioning;
                }
                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVRESSConditioning_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVRESSConditioning"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVRESSConditioning;
                }
                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVErrorCode"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVErrorCode;
                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVRESSSOC"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVRESSSOC;

                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Multiplier"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Multiplier;
                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Unit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Unit"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Unit;
                }
                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Value"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Value;

                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Multiplier"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Multiplier;
                    if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Unit_isUsed) {
                        outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Unit"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Unit;
                    }
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Value"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Value;
                }

                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Multiplier"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Multiplier;
                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Unit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Unit"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Unit;
                }
                outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Value"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Value;
                
                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Multiplier"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Multiplier;
                    if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Unit_isUsed) {
                        outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Unit"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Unit;
                    }
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Value"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Value;
                }

                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyRequest_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyRequest"]["Multiplier"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyRequest.Multiplier;
                    if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyRequest.Unit_isUsed) {
                        outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyRequest"]["Unit"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyRequest.Unit;
                    }
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyRequest"]["Value"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyRequest.Value;
                }

                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.FullSOC_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["FullSOC"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.FullSOC;
                }

                if (body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.BulkSOC_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["BulkSOC"] = body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.BulkSOC;
                }
            }

            if (body.ChargeParameterDiscoveryReq.EVChargeParameter_isUsed) {
                outJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter_isUsed"]["_unused"] = body.ChargeParameterDiscoveryReq.EVChargeParameter._unused;
            }
        } else if (body.ChargeParameterDiscoveryRes_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["ChargeParameterDiscoveryRes"]["isUsed"] = true;
            outJson["Body"]["ChargeParameterDiscoveryRes"]["ResponseCode"] = body.ChargeParameterDiscoveryRes.ResponseCode;
            outJson["Body"]["ChargeParameterDiscoveryRes"]["EVSEProcessing"] = body.ChargeParameterDiscoveryRes.EVSEProcessing;

            if (body.ChargeParameterDiscoveryRes.SAScheduleList_isUsed) {
                for (int i=0; i<body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.arrayLen; i++) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SAScheduleTupleID"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SAScheduleTupleID;
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleID"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleID;
                    for (int j=0; j<body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.arrayLen; j++) {
                        if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval_isUsed){
                            outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["RelativeTimeInterval"]["start"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.start;
                            if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.duration_isUsed){
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["RelativeTimeInterval"]["duration"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.duration;
                            }
                        }
                        if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].TimeInterval_isUsed) {
                            outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["TimeInterval"]["_unused"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].TimeInterval._unused;
                        }
                        outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["PMax"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].PMax;
                    }
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["arrayLen"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.arrayLen;

                    if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff_isUsed) {
                        outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["Id"]["characters"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id.characters;
                        outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["Id"]["charactersLen"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id.charactersLen;

                        outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffID"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffID;

                        if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription_isUsed){
                            outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffDescription"]["characters"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription.characters;
                            outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffDescription"]["charactersLen"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription.charactersLen;
                        }

                        outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["NumEPriceLevels"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.NumEPriceLevels;

                        for (int j=0; j<body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.arrayLen; j++) {
                            if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval_isUsed) {
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["RealtiveTimeInterval"]["start"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval.start;
                                if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval.duration_isUsed) {
                                    outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["RealtiveTimeInterval"]["duration"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].RelativeTimeInterval.duration;
                                }
                            }
                            if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].TimeInterval_isUsed) {
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["TimeInterval"]["_unused"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].TimeInterval._unused;
                            }
                            outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["EPriceLevel"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].EPriceLevel;
                            if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost_isUsed) {
                                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["startValue"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.startValue;
                                if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.Cost_isUsed) {
                                    outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["Cost"]["costKind"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.Cost.costKind;
                                    outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["Cost"]["amount"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.Cost.amount;
                                    if (body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.Cost.amountMultiplier_isUsed) {
                                        outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["Cost"]["amountMultiplier"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.Cost.amountMultiplier;
                                    }
                                }
                            }
                        }
                    }
                }
                outJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["arrayLen"] = body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.arrayLen;
            }

            if (body.ChargeParameterDiscoveryRes.SASchedules_isUsed) {
                outJson["Body"]["ChargeParameterDiscoveryRes"]["SASchedules"]["_unused"] = body.ChargeParameterDiscoveryRes.SASchedules._unused;
            }

            if (body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter_isUsed) {
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["PowerSwitchClosed"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.PowerSwitchClosed;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["RCD"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.RCD;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["NotificationMaxDelay"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["EVSENotification"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.EVSENotification;

                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxVoltage"]["Multiplier"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxVoltage.Multiplier;
                if (body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxVoltage.Unit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxVoltage"]["Unit"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxVoltage.Unit;
                }
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxVoltage"]["Value"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxVoltage.Value;

                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"]["Multiplier"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Multiplier;
                if (body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Unit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"]["Unit"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Unit;
                }
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"]["Value"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Value;

                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMinCurrent"]["Multiplier"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMinCurrent.Multiplier;
                if (body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMinCurrent.Unit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMinCurrent"]["Unit"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMinCurrent.Unit;
                }
                outJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMinCurrent"]["Value"] = body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMinCurrent.Value;
            }

            if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter_isUsed) {
                if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEIsolationStatus_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEIsolationStatus;
                }
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEStatusCode;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["EVSENotification"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSENotification;

                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Multiplier;
                if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Unit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Unit;
                }
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Value;

                if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Multiplier;
                    if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Unit_isUsed) {
                        outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Unit;
                    }
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Value;
                }

                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Multiplier;
                if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Unit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Unit;
                }
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Value;

                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Multiplier;
                if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Unit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Unit;
                }
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Value;
                
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Multiplier;
                if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Unit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Unit;
                }
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Value;

                if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Multiplier;
                    if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Unit_isUsed) {
                        outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Unit;
                    }
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Value;
                }

                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Multiplier;
                if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Unit_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Unit;
                }
                outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Value;
                
                if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered_isUsed) {
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"]["Multiplier"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Multiplier;
                    if (body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Unit_isUsed) {
                        outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"]["Unit"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Unit;
                    }
                    outJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"]["Value"] = body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Value;
                }
            }

            if (body.ChargeParameterDiscoveryRes.EVSEChargeParameter_isUsed) {
                outJson["Body"]["ChargeParameterDiscoveryRes"]["EVSEChargeParameter"]["_unused"] = body.ChargeParameterDiscoveryRes.EVSEChargeParameter._unused;
            }
        } else if (body.ChargingStatusReq_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.ChargingStatusRes_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.ContractAuthenticationReq_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["ContractAuthenticationReq"]["isUsed"] = true;

            if (body.ContractAuthenticationReq.Id_isUsed) {
                outJson["Body"]["ContractAuthenticationReq"]["Id"]["characters"] = body.ContractAuthenticationReq.Id.characters;
                outJson["Body"]["ContractAuthenticationReq"]["Id"]["charactersLen"] = body.ContractAuthenticationReq.Id.charactersLen;
            }

            if (body.ContractAuthenticationReq.GenChallenge_isUsed) {
                outJson["Body"]["ContractAuthenticationReq"]["GenChallenge"]["characters"] = body.ContractAuthenticationReq.GenChallenge.characters;
                outJson["Body"]["ContractAuthenticationReq"]["GenChallenge"]["charactersLen"] = body.ContractAuthenticationReq.GenChallenge.charactersLen;
            }
        } else if (body.ContractAuthenticationRes_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["ContractAuthenticationRes"]["isUsed"] = true;
            outJson["Body"]["ContractAuthenticationRes"]["ResponseCode"] = body.ContractAuthenticationRes.ResponseCode;
            outJson["Body"]["ContractAuthenticationRes"]["EVSEProcessing"] = body.ContractAuthenticationRes.EVSEProcessing;
        } else if (body.CurrentDemandReq_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["CurrentDemandReq"]["isUsed"] = true;

            outJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVReady"] = body.CurrentDemandReq.DC_EVStatus.EVReady;
            if (body.CurrentDemandReq.DC_EVStatus.EVCabinConditioning_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVCabinConditioning"] = body.CurrentDemandReq.DC_EVStatus.EVCabinConditioning;
            }
            if (body.CurrentDemandReq.DC_EVStatus.EVRESSConditioning_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVRESSConditioning"] = body.CurrentDemandReq.DC_EVStatus.EVRESSConditioning;
            }
            outJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVErrorCode"] = body.CurrentDemandReq.DC_EVStatus.EVErrorCode;
            outJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVRESSSOC"] = body.CurrentDemandReq.DC_EVStatus.EVRESSSOC;

            outJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"]["Multiplier"] = body.CurrentDemandReq.EVTargetCurrent.Multiplier;
            if (body.CurrentDemandReq.EVTargetCurrent.Unit_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"]["Unit"] = body.CurrentDemandReq.EVTargetCurrent.Unit;
            }
            outJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"]["Value"] = body.CurrentDemandReq.EVTargetCurrent.Value;

            if (body.CurrentDemandReq.EVMaximumVoltageLimit_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"]["Multiplier"] = body.CurrentDemandReq.EVMaximumVoltageLimit.Multiplier;
                if (body.CurrentDemandReq.EVMaximumVoltageLimit.Unit_isUsed) {
                    outJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"]["Unit"] = body.CurrentDemandReq.EVMaximumVoltageLimit.Unit;
                }
                outJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"]["Value"] = body.CurrentDemandReq.EVMaximumVoltageLimit.Value;
            }

            if (body.CurrentDemandReq.EVMaximumCurrentLimit_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"]["Multiplier"] = body.CurrentDemandReq.EVMaximumCurrentLimit.Multiplier;
                if (body.CurrentDemandReq.EVMaximumCurrentLimit.Unit_isUsed) {
                    outJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"]["Unit"] = body.CurrentDemandReq.EVMaximumCurrentLimit.Unit;
                }
                outJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"]["Value"] = body.CurrentDemandReq.EVMaximumCurrentLimit.Value;
            }

            if (body.CurrentDemandReq.EVMaximumPowerLimit_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"]["Multiplier"] = body.CurrentDemandReq.EVMaximumPowerLimit.Multiplier;
                if (body.CurrentDemandReq.EVMaximumPowerLimit.Unit_isUsed) {
                    outJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"]["Unit"] = body.CurrentDemandReq.EVMaximumPowerLimit.Unit;
                }
                outJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"]["Value"] = body.CurrentDemandReq.EVMaximumPowerLimit.Value;
            }

            if (body.CurrentDemandReq.BulkChargingComplete_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["BulkChargingComplete"] = body.CurrentDemandReq.BulkChargingComplete;
            }

            outJson["Body"]["CurrentDemandReq"]["ChargingComplete"] = body.CurrentDemandReq.ChargingComplete;

            if (body.CurrentDemandReq.RemainingTimeToFullSoC_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"]["Multiplier"] = body.CurrentDemandReq.RemainingTimeToFullSoC.Multiplier;
                if (body.CurrentDemandReq.RemainingTimeToFullSoC.Unit_isUsed) {
                    outJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"]["Unit"] = body.CurrentDemandReq.RemainingTimeToFullSoC.Unit;
                }
                outJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"]["Value"] = body.CurrentDemandReq.RemainingTimeToFullSoC.Value;
            }

            if (body.CurrentDemandReq.RemainingTimeToBulkSoC_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"]["Multiplier"] = body.CurrentDemandReq.RemainingTimeToBulkSoC.Multiplier;
                if (body.CurrentDemandReq.RemainingTimeToBulkSoC.Unit_isUsed) {
                    outJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"]["Unit"] = body.CurrentDemandReq.RemainingTimeToBulkSoC.Unit;
                }
                outJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"]["Value"] = body.CurrentDemandReq.RemainingTimeToBulkSoC.Value;
            }

            outJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"]["Multiplier"] = body.CurrentDemandReq.EVTargetVoltage.Multiplier;
            if (body.CurrentDemandReq.EVTargetVoltage.Unit_isUsed) {
                outJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"]["Unit"] = body.CurrentDemandReq.EVTargetVoltage.Unit;
            }
            outJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"]["Value"] = body.CurrentDemandReq.EVTargetVoltage.Value;
        } else if (body.CurrentDemandRes_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["CurrentDemandRes"]["isUsed"] = true;
            outJson["Body"]["CurrentDemandRes"]["ResponseCode"] = body.CurrentDemandRes.ResponseCode;

            if (body.CurrentDemandRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed) {
                outJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.CurrentDemandRes.DC_EVSEStatus.EVSEIsolationStatus;
            }
            outJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.CurrentDemandRes.DC_EVSEStatus.EVSEStatusCode;
            outJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.CurrentDemandRes.DC_EVSEStatus.NotificationMaxDelay;
            outJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["EVSENotification"] = body.CurrentDemandRes.DC_EVSEStatus.EVSENotification;
            
            outJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"]["Multiplier"] = body.CurrentDemandRes.EVSEPresentVoltage.Multiplier;
            if (body.CurrentDemandRes.EVSEPresentVoltage.Unit_isUsed) {
                outJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"]["Unit"] = body.CurrentDemandRes.EVSEPresentVoltage.Unit;
            }
            outJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"]["Value"] = body.CurrentDemandRes.EVSEPresentVoltage.Value;

            outJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"]["Multiplier"] = body.CurrentDemandRes.EVSEPresentCurrent.Multiplier;
            if (body.CurrentDemandRes.EVSEPresentCurrent.Unit_isUsed) {
                outJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"]["Unit"] = body.CurrentDemandRes.EVSEPresentCurrent.Unit;
            }
            outJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"]["Value"] = body.CurrentDemandRes.EVSEPresentCurrent.Value;

            outJson["Body"]["CurrentDemandRes"]["EVSECurrentLimitAchieved"] = body.CurrentDemandRes.EVSECurrentLimitAchieved;
            outJson["Body"]["CurrentDemandRes"]["EVSEPowerLimitAchieved"] = body.CurrentDemandRes.EVSEPowerLimitAchieved;
            outJson["Body"]["CurrentDemandRes"]["EVSEVoltageLimitAchieved"] = body.CurrentDemandRes.EVSEVoltageLimitAchieved;

            if (body.CurrentDemandRes.EVSEMaximumVoltageLimit_isUsed) {
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"]["Multiplier"] = body.CurrentDemandRes.EVSEMaximumVoltageLimit.Multiplier;
                if (body.CurrentDemandRes.EVSEMaximumVoltageLimit.Unit_isUsed) {
                    outJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"]["Unit"] = body.CurrentDemandRes.EVSEMaximumVoltageLimit.Unit;
                }
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"]["Value"] = body.CurrentDemandRes.EVSEMaximumVoltageLimit.Value;
            }

            if (body.CurrentDemandRes.EVSEMaximumCurrentLimit_isUsed) {
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"]["Multiplier"] = body.CurrentDemandRes.EVSEMaximumCurrentLimit.Multiplier;
                if (body.CurrentDemandRes.EVSEMaximumCurrentLimit.Unit_isUsed) {
                    outJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"]["Unit"] = body.CurrentDemandRes.EVSEMaximumCurrentLimit.Unit;
                }
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"]["Value"] = body.CurrentDemandRes.EVSEMaximumCurrentLimit.Value;
            }

            if (body.CurrentDemandRes.EVSEMaximumPowerLimit_isUsed) {
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"]["Multiplier"] = body.CurrentDemandRes.EVSEMaximumPowerLimit.Multiplier;
                if (body.CurrentDemandRes.EVSEMaximumPowerLimit.Unit_isUsed) {
                    outJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"]["Unit"] = body.CurrentDemandRes.EVSEMaximumPowerLimit.Unit;
                }
                outJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"]["Value"] = body.CurrentDemandRes.EVSEMaximumPowerLimit.Value;
            }
        } else if (body.MeteringReceiptReq_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.MeteringReceiptRes_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.PaymentDetailsReq_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.PaymentDetailsRes_isUsed) {/////////////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.PowerDeliveryReq_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["PowerDeliveryReq"]["isUsed"] = true;

            outJson["Body"]["PowerDeliveryReq"]["ReadyToChargeState"] = body.PowerDeliveryReq.ReadyToChargeState;

            if (body.PowerDeliveryReq.ChargingProfile_isUsed) {
                outJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["SAScheduleTupleID"] = body.PowerDeliveryReq.ChargingProfile.SAScheduleTupleID;
                for (int i=0; i<body.PowerDeliveryReq.ChargingProfile.ProfileEntry.arrayLen; i++) {
                    outJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryStart"] = body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryStart;
                    outJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryMaxPower"] = body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxPower;
                }
                outJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["arrayLen"] = body.PowerDeliveryReq.ChargingProfile.ProfileEntry.arrayLen;
            }

            if (body.PowerDeliveryReq.DC_EVPowerDeliveryParameter_isUsed) {
                outJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVReady"] = body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVReady;
                if (body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVCabinConditioning_isUsed) {
                    outJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVCabinConditioning"] = body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVCabinConditioning;
                }
                if (body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVRESSConditioning_isUsed) {
                    outJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVRESSConditioning"] = body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVRESSConditioning;
                }
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
        } else if (body.PowerDeliveryRes_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["PowerDeliveryRes"]["isUsed"] = true;
            outJson["Body"]["PowerDeliveryRes"]["ResponseCode"] = body.PowerDeliveryRes.ResponseCode;

            if (body.PowerDeliveryRes.AC_EVSEStatus_isUsed) {
                outJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["PowerSwitchClosed"] = body.PowerDeliveryRes.AC_EVSEStatus.PowerSwitchClosed;
                outJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["RCD"] = body.PowerDeliveryRes.AC_EVSEStatus.RCD;
                outJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["NotificationMaxDelay"] = body.PowerDeliveryRes.AC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["EVSENotification"] = body.PowerDeliveryRes.AC_EVSEStatus.EVSENotification;
            }

            if (body.PowerDeliveryRes.DC_EVSEStatus_isUsed) {
                outJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.PowerDeliveryRes.DC_EVSEStatus.EVSEIsolationStatus;
                outJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.PowerDeliveryRes.DC_EVSEStatus.EVSEStatusCode;
                outJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.PowerDeliveryRes.DC_EVSEStatus.NotificationMaxDelay;
                outJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["EVSENotification"] = body.PowerDeliveryRes.DC_EVSEStatus.EVSENotification;
            }

            if (body.PowerDeliveryRes.EVSEStatus_isUsed) {
                outJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["_unused"] = body.PowerDeliveryRes.EVSEStatus._unused;
            }
        } else if (body.PreChargeReq_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["PreChargeReq"]["isUsed"] = true;

            outJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVReady"] = body.PreChargeReq.DC_EVStatus.EVReady;
            if (body.PreChargeReq.DC_EVStatus.EVCabinConditioning_isUsed) {
                outJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVCabinConditioning"] = body.PreChargeReq.DC_EVStatus.EVCabinConditioning;
            }
            if (body.PreChargeReq.DC_EVStatus.EVRESSConditioning_isUsed) {
                outJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVRESSConditioning"] = body.PreChargeReq.DC_EVStatus.EVRESSConditioning;
            }
            outJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVErrorCode"] = body.PreChargeReq.DC_EVStatus.EVErrorCode;
            outJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVRESSSOC"] = body.PreChargeReq.DC_EVStatus.EVRESSSOC;

            outJson["Body"]["PreChargeReq"]["EVTargetVoltage"]["Multiplier"] = body.PreChargeReq.EVTargetVoltage.Multiplier;
            if (body.PreChargeReq.EVTargetVoltage.Unit_isUsed) {
                outJson["Body"]["PreChargeReq"]["EVTargetVoltage"]["Unit"] = body.PreChargeReq.EVTargetVoltage.Unit;
            }
            outJson["Body"]["PreChargeReq"]["EVTargetVoltage"]["Value"] = body.PreChargeReq.EVTargetVoltage.Value;

            outJson["Body"]["PreChargeReq"]["EVTargetCurrent"]["Multiplier"] = body.PreChargeReq.EVTargetCurrent.Multiplier;
            if (body.PreChargeReq.EVTargetCurrent.Unit_isUsed) {
                outJson["Body"]["PreChargeReq"]["EVTargetCurrent"]["Unit"] = body.PreChargeReq.EVTargetCurrent.Unit;
            }
            outJson["Body"]["PreChargeReq"]["EVTargetCurrent"]["Value"] = body.PreChargeReq.EVTargetCurrent.Value;
        } else if (body.PreChargeRes_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["PreChargeRes"]["isUsed"] = true;

            outJson["Body"]["PreChargeRes"]["ResponseCode"] = body.PreChargeRes.ResponseCode;

            outJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"] = body.PreChargeRes.DC_EVSEStatus.EVSEIsolationStatus;
            outJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["EVSEStatusCode"] = body.PreChargeRes.DC_EVSEStatus.EVSEStatusCode;
            outJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["NotificationMaxDelay"] = body.PreChargeRes.DC_EVSEStatus.NotificationMaxDelay;
            outJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["EVSENotification"] = body.PreChargeRes.DC_EVSEStatus.EVSENotification;

            outJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"]["Multiplier"] = body.PreChargeRes.EVSEPresentVoltage.Multiplier;
            if (body.PreChargeRes.EVSEPresentVoltage.Unit_isUsed) {
                outJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"]["Unit"] = body.PreChargeRes.EVSEPresentVoltage.Unit;
            }
            outJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"]["Value"] = body.PreChargeRes.EVSEPresentVoltage.Value;
        } else if (body.ServiceDetailReq_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.ServiceDetailRes_isUsed) {//////////////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.ServiceDiscoveryReq_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["ServiceDiscoveryReq"]["isUsed"] = true;

            if (body.ServiceDiscoveryReq.ServiceScope_isUsed){
                outJson["Body"]["ServiceDiscoveryReq"]["ServiceScope"]["characters"] = body.ServiceDiscoveryReq.ServiceScope.characters;
                outJson["Body"]["ServiceDiscoveryReq"]["ServiceScope"]["charactersLen"] = body.ServiceDiscoveryReq.ServiceScope.charactersLen;
            }

            if (body.ServiceDiscoveryReq.ServiceCategory_isUsed) {
                outJson["Body"]["ServiceDiscoveryReq"]["ServiceCategory"] = body.ServiceDiscoveryReq.ServiceCategory;
            }
        } else if (body.ServiceDiscoveryRes_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["ServiceDiscoveryRes"]["isUsed"] = true;
            outJson["Body"]["ServiceDiscoveryRes"]["ResponseCode"] = body.ServiceDiscoveryRes.ResponseCode;

            outJson["Body"]["ServiceDiscoveryRes"]["PaymentOptions"]["PaymentOption"]["arrayLen"] = body.ServiceDiscoveryRes.PaymentOptions.PaymentOption.arrayLen;
            for (int i=0; i<body.ServiceDiscoveryRes.PaymentOptions.PaymentOption.arrayLen; i++) {
                outJson["Body"]["ServiceDiscoveryRes"]["PaymentOptions"]["PaymentOption"]["array"][i] = body.ServiceDiscoveryRes.PaymentOptions.PaymentOption.array[i];
            }

            outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"]["ServiceID"] = body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceID;
            if (body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceName_isUsed) {
                outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"]["ServiceName"]["charactersLen"] = body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceName.charactersLen;
                for (int i=0; i<body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceName.charactersLen; i++) {
                    outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"]["ServiceName"]["characters"][i] = body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceName.characters[i];
                }
            }
            outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"]["ServiceCategory"] = body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceCategory;
            if (body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceScope_isUsed) {
                outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"]["ServiceScope"]["charactersLen"] = body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceScope.charactersLen;
                for (int i=0; i<body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceScope.charactersLen; i++) {
                    outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"]["ServiceScope"]["characters"][i] = body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceScope.characters[i];
                }
            }
            outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["FreeService"] = body.ServiceDiscoveryRes.ChargeService.FreeService;
            outJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["EnergyTransferType"] = body.ServiceDiscoveryRes.ChargeService.EnergyTransferType;

            if (body.ServiceDiscoveryRes.ServiceList_isUsed) {
                outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"]["ServiceID"] = body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceID;
                if (body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceName_isUsed) {
                    outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"]["ServiceName"]["charactersLen"] = body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceName.charactersLen;
                    for (int i=0; i<body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceName.charactersLen; i++) {
                        outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"]["ServiceName"]["characters"][i] = body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceName.characters[i];
                    }
                }
                outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"]["ServiceCategory"] = body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceCategory;
                if (body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceScope_isUsed) {
                    outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"]["ServiceScope"]["charactersLen"] = body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceScope.charactersLen;
                    for (int i=0; i<body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceScope.charactersLen; i++) {
                        outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"]["ServiceScope"]["characters"][i] = body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceScope.characters[i];
                    }
                }
                outJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["FreeService"] = body.ServiceDiscoveryRes.ServiceList.Service.FreeService;
            }
        } else if (body.ServicePaymentSelectionReq_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["ServicePaymentSelectionReq"]["isUsed"] = true;
            outJson["Body"]["ServicePaymentSelectionReq"]["SelectedPaymentOption"] = body.ServicePaymentSelectionReq.SelectedPaymentOption;

            for (int i=0; i<body.ServicePaymentSelectionReq.SelectedServiceList.SelectedService.arrayLen; i++) {
                outJson["Body"]["ServicePaymentSelectionReq"]["SelectedServiceList"]["SelectedService"]["array"][i]["ServiceID"] = body.ServicePaymentSelectionReq.SelectedServiceList.SelectedService.array[i].ServiceID;
                if (body.ServicePaymentSelectionReq.SelectedServiceList.SelectedService.array[i].ParameterSetID_isUsed) {
                    outJson["Body"]["ServicePaymentSelectionReq"]["SelectedServiceList"]["SelectedService"]["array"][i]["ParameterSetID"] = body.ServicePaymentSelectionReq.SelectedServiceList.SelectedService.array[i].ParameterSetID;
                }
            }
            outJson["Body"]["ServicePaymentSelectionReq"]["SelectedServiceList"]["SelectedService"]["arrayLen"] = body.ServicePaymentSelectionReq.SelectedServiceList.SelectedService.arrayLen;
        } else if (body.ServicePaymentSelectionRes_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["ServicePaymentSelectionRes"]["isUsed"] = true;
            outJson["Body"]["ServicePaymentSelectionRes"]["ResponseCode"] = body.ServicePaymentSelectionRes.ResponseCode;
        } else if (body.SessionSetupReq_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["SessionSetupReq"]["isUsed"] = true;
            outJson["Body"]["SessionSetupReq"]["EVCCID"]["bytes"] = body.SessionSetupReq.EVCCID.bytes;
            outJson["Body"]["SessionSetupReq"]["EVCCID"]["bytesLen"] = body.SessionSetupReq.EVCCID.bytesLen;
        } else if (body.SessionSetupRes_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["SessionSetupRes"]["isUsed"] = true;

            outJson["Body"]["SessionSetupRes"]["ResponseCode"] = body.SessionSetupRes.ResponseCode;

            outJson["Body"]["SessionSetupRes"]["EVSEID"]["bytesLen"] = body.SessionSetupRes.EVSEID.bytesLen;
            for (int i=0; i<body.SessionSetupRes.EVSEID.bytesLen; i++) {
                outJson["Body"]["SessionSetupRes"]["EVSEID"]["bytes"][i] = body.SessionSetupRes.EVSEID.bytes[i];
            }
            
            if (body.SessionSetupRes.DateTimeNow_isUsed) {
                outJson["Body"]["SessionSetupRes"]["DateTimeNow"] = body.SessionSetupRes.DateTimeNow;
            }
        } else if (body.SessionStopReq_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["SessionStopReq"]["isUsed"] = true;
            outJson["Body"]["SessionStopReq"]["_unused"] = body.SessionStopReq._unused;
        } else if (body.SessionStopRes_isUsed) {////////////////////////////////////////////////////////////////////////////////////////////////////////////
            outJson["Body"]["SessionStopRes"]["isUsed"] = true;
            outJson["Body"]["SessionStopRes"]["ResponseCode"] = body.SessionStopRes.ResponseCode;
        } else if (body.WeldingDetectionReq_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
        } else if (body.WeldingDetectionRes_isUsed) {///////////////////////////////////////////////////////////////////////////////////////////////////////
            return "NOT IMPLEMENTED";
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

        din_exiDocument inDoc;
        init_din_exiDocument(&inDoc);
        init_din_MessageHeaderType(&inDoc.V2G_Message.Header);

        auto& header = inDoc.V2G_Message.Header;
        header.SessionID.bytesLen = din_sessionIDType_BYTES_SIZE;
        copy(sessionID.begin(), sessionID.end(), header.SessionID.bytes);

        init_din_BodyType(&inDoc.V2G_Message.Body);
        auto& body = inDoc.V2G_Message.Body;

        string packetType = inJson["Body"].items().begin().key();

        if (packetType == "CableCheckReq") {////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_CableCheckReqType(&body.CableCheckReq);
            body.CableCheckReq_isUsed = true;

            body.CableCheckReq.DC_EVStatus.EVReady = inJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVReady"].template get<int>();

            if (inJson["Body"]["CableCheckReq"]["DC_EVStatus"].contains("EVCabinConditioning")) {
                body.CableCheckReq.DC_EVStatus.EVCabinConditioning_isUsed = true;
                body.CableCheckReq.DC_EVStatus.EVCabinConditioning = inJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVCabinConditioning"].template get<int>();
            } else {
                body.CableCheckReq.DC_EVStatus.EVCabinConditioning_isUsed = false;
            }

            if (inJson["Body"]["CableCheckReq"]["DC_EVStatus"].contains("EVRESSConditioning")) {
                cout << "EVRESSConditioning" << endl;
                body.CableCheckReq.DC_EVStatus.EVRESSConditioning_isUsed = true;
                body.CableCheckReq.DC_EVStatus.EVRESSConditioning = inJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVRESSConditioning"].template get<int>();
            } else {
                body.CableCheckReq.DC_EVStatus.EVRESSConditioning_isUsed = false;
            }

            body.CableCheckReq.DC_EVStatus.EVErrorCode = static_cast<din_DC_EVErrorCodeType>(inJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVErrorCode"].template get<uint8_t>());

            body.CableCheckReq.DC_EVStatus.EVRESSSOC = inJson["Body"]["CableCheckReq"]["DC_EVStatus"]["EVRESSSOC"].template get<uint8_t>();

        } else if (packetType == "CableCheckRes") {/////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_CableCheckResType(&body.CableCheckRes);
            body.CableCheckRes_isUsed = true;
            body.CableCheckRes.ResponseCode = static_cast<din_responseCodeType>(inJson["Body"]["CableCheckRes"]["ResponseCode"].template get<int>());

            if (inJson["Body"]["CableCheckRes"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                body.CableCheckRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                body.CableCheckRes.DC_EVSEStatus.EVSEIsolationStatus = static_cast<din_isolationLevelType>(inJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
            } else {
                body.CableCheckRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
            }
            
            body.CableCheckRes.DC_EVSEStatus.EVSEStatusCode = static_cast<din_DC_EVSEStatusCodeType>(inJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());
            body.CableCheckRes.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint32_t>();
            body.CableCheckRes.DC_EVSEStatus.EVSENotification = static_cast<din_EVSENotificationType>(inJson["Body"]["CableCheckRes"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());

            body.CableCheckRes.EVSEProcessing = static_cast<din_EVSEProcessingType>(inJson["Body"]["CableCheckRes"]["EVSEProcessing"].template get<int>());
        } else if (packetType == "CertificateInstallReq") {/////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "CertificateInstallReq") {/////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "CertificateInstallRes") {/////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "CertificateUpdateReq") {//////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "CertificateUpdateRes") {//////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "ChargeParameterDiscoveryReq") {///////////////////////////////////////////////////////////////////////////////////////////
            init_din_ChargeParameterDiscoveryReqType(&body.ChargeParameterDiscoveryReq);
            body.ChargeParameterDiscoveryReq_isUsed = true;

            body.ChargeParameterDiscoveryReq.EVRequestedEnergyTransferType = static_cast<din_EVRequestedEnergyTransferType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["EVRequestedEnergyTransferType"].template get<int>());

            if (inJson["Body"]["ChargeParameterDiscoveryReq"].contains("AC_EVChargeParameter")) {
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter_isUsed = true;
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.DepartureTime = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["DepartureTime"].template get<uint32_t>();

                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EAmount.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EAmount"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EAmount"].contains("Unit")) {
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EAmount.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EAmount.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EAmount"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EAmount.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EAmount.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EAmount"]["Value"].template get<uint16_t>();
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter_isUsed = false;

                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxVoltage.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxVoltage"].contains("Unit")) {
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxVoltage.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxVoltage.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxVoltage.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxVoltage.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxVoltage"]["Value"].template get<uint16_t>();

                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxCurrent.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxCurrent"].contains("Unit")) {
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxCurrent.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxCurrent.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxCurrent.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMaxCurrent.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMaxCurrent"]["Value"].template get<uint16_t>();

                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMinCurrent.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMinCurrent"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMinCurrent"].contains("Unit")) {
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMinCurrent.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMinCurrent.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMinCurrent"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMinCurrent.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter.EVMinCurrent.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["AC_EVChargeParameter"]["EVMinCurrent"]["Value"].template get<uint16_t>();
            } else {
                body.ChargeParameterDiscoveryReq.AC_EVChargeParameter_isUsed = false;
            }

            if (inJson["Body"]["ChargeParameterDiscoveryReq"].contains("DC_EVChargeParameter")) {
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter_isUsed = true;

                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVReady = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVReady"].template get<int>();
                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"].contains("EVCabinConditioning")) {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVCabinConditioning_isUsed = true;
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVCabinConditioning = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVCabinConditioning"].template get<int>();
                } else {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVCabinConditioning_isUsed = false;
                }
                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"].contains("EVRESSConditioning")) {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVRESSConditioning_isUsed = true;
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVRESSConditioning = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVRESSConditioning"].template get<int>();
                } else {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVRESSConditioning_isUsed = false;
                }
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVErrorCode = static_cast<din_DC_EVErrorCodeType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVErrorCode"].template get<uint8_t>());
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.DC_EVStatus.EVRESSSOC = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["DC_EVStatus"]["EVRESSSOC"].template get<uint8_t>();

                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"].contains("Unit")) {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumCurrentLimit.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumCurrentLimit"]["Value"].template get<uint16_t>();

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"].contains("EVMaximumPowerLimit")) {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit_isUsed = true;
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Multiplier"].template get<uint8_t>();
                    if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"].contains("Unit")) {
                        body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Unit_isUsed = true;
                        body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Unit"].template get<int>());
                    } else {
                        body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Unit_isUsed = false;
                    }
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumPowerLimit"]["Value"].template get<uint16_t>();
                } else {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumPowerLimit_isUsed = false;
                }

                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"].contains("Unit")) {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVMaximumVoltageLimit.Value = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVMaximumVoltageLimit"]["Value"].template get<uint16_t>();

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"].contains("EVEnergyCapacity")) {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity_isUsed = true;
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Multiplier"].template get<uint8_t>();
                    if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyCapacity"].contains("Unit")) {
                        body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Unit_isUsed = true;
                        body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyCapacity"]["Unit"].template get<int>());
                    } else {
                        body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity.Unit_isUsed = false;
                    }
                } else {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyCapacity_isUsed = false;
                }

                if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"].contains("EVEnergyRequest")) {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyRequest_isUsed = true;
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyRequest.Multiplier = inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyRequest"]["Multiplier"].template get<uint8_t>();
                    if (inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyRequest"].contains("Unit")) {
                        body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyRequest.Unit_isUsed = true;
                        body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyRequest.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryReq"]["DC_EVChargeParameter"]["EVEnergyRequest"]["Unit"].template get<int>());
                    } else {
                        body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyRequest.Unit_isUsed = false;
                    }
                } else {
                    body.ChargeParameterDiscoveryReq.DC_EVChargeParameter.EVEnergyRequest_isUsed = false;
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
                // body.ChargeParameterDiscoveryReq.EVChargeParameter_isUsed._unused = inJson["Body"]["ChargeParameterDiscoveryReq"]["EVChargeParameter_isUsed"]["_unused"];
            } else {
                body.ChargeParameterDiscoveryReq.EVChargeParameter_isUsed = false;
            }
            
        } else if (packetType == "ChargeParameterDiscoveryRes") {///////////////////////////////////////////////////////////////////////////////////////////
            init_din_ChargeParameterDiscoveryResType(&body.ChargeParameterDiscoveryRes);
            body.ChargeParameterDiscoveryRes_isUsed = true;

            body.ChargeParameterDiscoveryRes.ResponseCode = static_cast<din_responseCodeType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["ResponseCode"].template get<int>());
            body.ChargeParameterDiscoveryRes.EVSEProcessing = static_cast<din_EVSEProcessingType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["EVSEProcessing"].template get<int>());

            if (inJson["Body"]["ChargeParameterDiscoveryRes"].contains("SAScheduleList")) {
                body.ChargeParameterDiscoveryRes.SAScheduleList_isUsed = true;
                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.arrayLen = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["arrayLen"].template get<uint8_t>();
                for (int i = 0; i < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.arrayLen; i++) {
                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SAScheduleTupleID = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SAScheduleTupleID"].template get<uint16_t>();
                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleID = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleID"].template get<uint16_t>();
                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.arrayLen = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["arrayLen"].template get<uint8_t>();
                    for (int j = 0; j < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.arrayLen; j++) {
                        if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j].contains("RelativeTimeInterval")) {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval_isUsed = true;
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.start = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["RelativeTimeInterval"]["start"].template get<uint32_t>();
                            if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["RelativeTimeInterval"].contains("duration")) {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.duration_isUsed = true;
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.duration = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["RelativeTimeInterval"]["duration"].template get<uint32_t>();
                            } else {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval.duration_isUsed = false;
                            }
                        } else {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].RelativeTimeInterval_isUsed = false;
                        }
                        if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j].contains("TimeInterval")){
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].TimeInterval_isUsed = true;
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].TimeInterval._unused = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["TimeInterval"]["_unused"].template get<int>();
                        } else {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].TimeInterval_isUsed = false;
                        }
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].PMaxSchedule.PMaxScheduleEntry.array[j].PMax = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["PMaxSchedule"]["PMaxScheduleEntry"]["array"][j]["PMax"].template get<uint16_t>();
                    }
                    if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i].contains("SalesTariff")) {
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff_isUsed = true;
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id.charactersLen = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["Id"]["charactersLen"].template get<uint16_t>();
                        for (int j = 0; j < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id.charactersLen; j++) {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.Id.characters[j] = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["Id"]["characters"][j].template get<uint8_t>();
                        }
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffID = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffID"].template get<uint16_t>();
                        if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"].contains("SalesTariffDescription")) {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription_isUsed = true;
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription.charactersLen = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffDescription"]["charactersLen"].template get<uint16_t>();
                            for (int j = 0; j < body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription.charactersLen; j++) {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription.characters[j] = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffDescription"]["characters"][j].template get<uint8_t>();
                            }
                        } else {
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffDescription_isUsed = false;
                        }
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.NumEPriceLevels = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["NumEPriceLevels"].template get<uint8_t>();
                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.arrayLen = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["arrayLen"].template get<uint16_t>();
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
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].TimeInterval._unused = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["TimeInterval"]["_unused"].template get<int>();
                            } else {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].TimeInterval_isUsed = false;
                            }
                            body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].EPriceLevel = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["EPriceLevel"].template get<uint8_t>();
                            if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j].contains("ConsumptionCost")) {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost_isUsed = true;
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.startValue = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["startValue"].template get<uint32_t>();
                                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"].contains("Cost")) {
                                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.Cost_isUsed = true;
                                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.Cost.costKind = static_cast<din_costKindType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["Cost"]["costKind"].template get<int>());
                                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.Cost.amount = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["Cost"]["amount"].template get<uint32_t>();
                                    if (inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["Cost"].contains("amountMultiplier")) {
                                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.Cost.amountMultiplier_isUsed = true;
                                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.Cost.amountMultiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["SAScheduleList"]["SAScheduleTuple"]["array"][i]["SalesTariff"]["SalesTariffEntry"]["array"][j]["ConsumptionCost"]["Cost"]["amountMultiplier"].template get<uint8_t>();
                                    } else {
                                        body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.Cost.amountMultiplier_isUsed = false;
                                    }
                                } else {
                                    body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost.Cost_isUsed = false;
                                }
                            } else {
                                body.ChargeParameterDiscoveryRes.SAScheduleList.SAScheduleTuple.array[i].SalesTariff.SalesTariffEntry.array[j].ConsumptionCost_isUsed = false;
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
                body.ChargeParameterDiscoveryRes.SASchedules._unused = inJson["Body"]["ChargeParameterDiscoveryRes"]["SASchedules"]["_unused"].template get<int>();
            } else {
                body.ChargeParameterDiscoveryRes.SASchedules_isUsed = false;
            }

            if (inJson["Body"]["ChargeParameterDiscoveryRes"].contains("AC_EVSEChargeParameter")) {
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter_isUsed = true;
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.PowerSwitchClosed = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["PowerSwitchClosed"].template get<int>();
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.RCD = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["RCD"].template get<int>();
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["NotificationMaxDelay"].template get<uint32_t>();
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.AC_EVSEStatus.EVSENotification = static_cast<din_EVSENotificationType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["AC_EVSEStatus"]["EVSENotification"].template get<int>());
            
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxVoltage.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxVoltage"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxVoltage"].contains("Unit")) {
                    body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxVoltage.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxVoltage.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxVoltage"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxVoltage.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxVoltage.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxVoltage"]["Value"].template get<uint16_t>();

                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"].contains("Unit")) {
                    body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMaxCurrent.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMaxCurrent"]["Value"].template get<uint16_t>();

                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMinCurrent.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMinCurrent"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMinCurrent"].contains("Unit")) {
                    body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMinCurrent.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMinCurrent.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMinCurrent"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMinCurrent.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter.EVSEMinCurrent.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["AC_EVSEChargeParameter"]["EVSEMinCurrent"]["Value"].template get<uint16_t>();
            } else {
                body.ChargeParameterDiscoveryRes.AC_EVSEChargeParameter_isUsed = false;
            }

            if (inJson["Body"]["ChargeParameterDiscoveryRes"].contains("DC_EVSEChargeParameter")) {
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter_isUsed = true;
                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEIsolationStatus = static_cast<din_isolationLevelType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
                }
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSEStatusCode = static_cast<din_DC_EVSEStatusCodeType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint32_t>();
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.DC_EVSEStatus.EVSENotification = static_cast<din_EVSENotificationType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());
            
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"].contains("Unit")) {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumCurrentLimit.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumCurrentLimit"]["Value"].template get<uint16_t>();

                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"].contains("EVSEMaximumPowerLimit")) {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit_isUsed = true;
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"]["Multiplier"].template get<uint8_t>();
                    if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"].contains("Unit")) {
                        body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Unit_isUsed = true;
                        body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"]["Unit"].template get<int>());
                    } else {
                        body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Unit_isUsed = false;
                    }
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumPowerLimit"]["Value"].template get<uint16_t>();
                } else {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumPowerLimit_isUsed = false;
                }

                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"].contains("Unit")) {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMaximumVoltageLimit.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMaximumVoltageLimit"]["Value"].template get<uint16_t>();

                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"].contains("Unit")) {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumCurrentLimit.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumCurrentLimit"]["Value"].template get<uint16_t>();

                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"].contains("Unit")) {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEMinimumVoltageLimit.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEMinimumVoltageLimit"]["Value"].template get<uint16_t>();

                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"].contains("EVSECurrentRegulationTolerance")) {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance_isUsed = true;
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"]["Multiplier"].template get<uint8_t>();
                    if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"].contains("Unit")) {
                        body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Unit_isUsed = true;
                        body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"]["Unit"].template get<int>());
                    } else {
                        body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Unit_isUsed = false;
                    }
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSECurrentRegulationTolerance"]["Value"].template get<uint16_t>();
                } else {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSECurrentRegulationTolerance_isUsed = false;
                }

                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"].contains("Unit")) {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Unit_isUsed = true;
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"]["Unit"].template get<int>());
                } else {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Unit_isUsed = false;
                }
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEPeakCurrentRipple.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEPeakCurrentRipple"]["Value"].template get<uint16_t>();

                if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"].contains("EVSEEnergyToBeDelivered")) {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered_isUsed = true;
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Multiplier = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"]["Multiplier"].template get<uint8_t>();
                    if (inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"].contains("Unit")) {
                        body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Unit_isUsed = true;
                        body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"]["Unit"].template get<int>());
                    } else {
                        body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Unit_isUsed = false;
                    }
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered.Value = inJson["Body"]["ChargeParameterDiscoveryRes"]["DC_EVSEChargeParameter"]["EVSEEnergyToBeDelivered"]["Value"].template get<uint16_t>();
                } else {
                    body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter.EVSEEnergyToBeDelivered_isUsed = false;
                }
            } else {
                body.ChargeParameterDiscoveryRes.DC_EVSEChargeParameter_isUsed = false;
            }

            if (inJson["Body"]["ChargeParameterDiscoveryRes"].contains("EVSEChargeParameter")) {
                body.ChargeParameterDiscoveryRes.EVSEChargeParameter_isUsed = true;
                body.ChargeParameterDiscoveryRes.EVSEChargeParameter._unused = inJson["Body"]["ChargeParameterDiscoveryRes"]["EVSEChargeParameter"]["_unused"].template get<int>();
            } else {
                body.ChargeParameterDiscoveryRes.EVSEChargeParameter_isUsed = false;
            }
        } else if (packetType == "ChargingStatusReq") {/////////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "ChargingStatusRes") {/////////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "ContractAuthenticationReq") {/////////////////////////////////////////////////////////////////////////////////////////////
            init_din_ContractAuthenticationReqType(&body.ContractAuthenticationReq);
            body.ContractAuthenticationReq_isUsed = true;

            if (inJson["Body"]["ContractAuthenticationReq"].contains("Id")) {
                body.ContractAuthenticationReq.Id_isUsed = true;
                body.ContractAuthenticationReq.Id.charactersLen = inJson["Body"]["ContractAuthenticationReq"]["Id"]["charactersLen"].template get<uint16_t>();
                for (int i=0; i<body.ContractAuthenticationReq.Id.charactersLen; i++){
                    body.ContractAuthenticationReq.Id.characters[i] = inJson["Body"]["ContractAuthenticationReq"]["Id"]["characters"][i].template get<uint8_t>();
                }
            } else {
                body.ContractAuthenticationReq.Id_isUsed = false;
            }

            if (inJson["Body"]["ContractAuthenticationReq"].contains("GenChallenge")) {
                body.ContractAuthenticationReq.GenChallenge_isUsed = true;
                body.ContractAuthenticationReq.GenChallenge.charactersLen = inJson["Body"]["ContractAuthenticationReq"]["GenChallenge"]["charactersLen"].template get<uint16_t>();
                for (int i=0; i<body.ContractAuthenticationReq.GenChallenge.charactersLen; i++){
                    body.ContractAuthenticationReq.GenChallenge.characters[i] = inJson["Body"]["ContractAuthenticationReq"]["GenChallenge"]["characters"][i].template get<uint8_t>();
                }
            } else {
                body.ContractAuthenticationReq.GenChallenge_isUsed = false;
            }
        } else if (packetType == "ContractAuthenticationRes") {/////////////////////////////////////////////////////////////////////////////////////////////
            init_din_ContractAuthenticationResType(&body.ContractAuthenticationRes);
            body.ContractAuthenticationRes_isUsed = true;

            body.ContractAuthenticationRes.ResponseCode = static_cast<din_responseCodeType>(inJson["Body"]["ContractAuthenticationRes"]["ResponseCode"].template get<int>());
            body.ContractAuthenticationRes.EVSEProcessing = static_cast<din_EVSEProcessingType>(inJson["Body"]["ContractAuthenticationRes"]["EVSEProcessing"].template get<int>());
        } else if (packetType == "CurrentDemandReq") {//////////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_CurrentDemandReqType(&body.CurrentDemandReq);
            body.CurrentDemandReq_isUsed = true;

            body.CurrentDemandReq.DC_EVStatus.EVReady = inJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVReady"].template get<int>();
            if (inJson["Body"]["CurrentDemandReq"]["DC_EVStatus"].contains("EVCabinConditioning")) {
                body.CurrentDemandReq.DC_EVStatus.EVCabinConditioning_isUsed = true;
                body.CurrentDemandReq.DC_EVStatus.EVCabinConditioning = inJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVCabinConditioning"].template get<int>();
            } else {
                body.CurrentDemandReq.DC_EVStatus.EVCabinConditioning_isUsed = false;
            }
            if (inJson["Body"]["CurrentDemandReq"]["DC_EVStatus"].contains("EVRESSConditioning")) {
                body.CurrentDemandReq.DC_EVStatus.EVRESSConditioning_isUsed = true;
                body.CurrentDemandReq.DC_EVStatus.EVRESSConditioning = inJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVRESSConditioning"].template get<int>();
            } else {
                body.CurrentDemandReq.DC_EVStatus.EVRESSConditioning_isUsed = false;
            }
            body.CurrentDemandReq.DC_EVStatus.EVErrorCode = static_cast<din_DC_EVErrorCodeType>(inJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVErrorCode"].template get<int>());
            body.CurrentDemandReq.DC_EVStatus.EVRESSSOC = inJson["Body"]["CurrentDemandReq"]["DC_EVStatus"]["EVRESSSOC"].template get<uint8_t>();

            body.CurrentDemandReq.EVTargetCurrent.Multiplier = inJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"]["Multiplier"].template get<uint8_t>();
            if (inJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"].contains("Unit")) {
                body.CurrentDemandReq.EVTargetCurrent.Unit_isUsed = true;
                body.CurrentDemandReq.EVTargetCurrent.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"]["Unit"].template get<int>());
            } else {
                body.CurrentDemandReq.EVTargetCurrent.Unit_isUsed = false;
            }
            body.CurrentDemandReq.EVTargetCurrent.Value = inJson["Body"]["CurrentDemandReq"]["EVTargetCurrent"]["Value"].template get<uint16_t>();

            if (inJson["Body"]["CurrentDemandReq"].contains("EVMaximumVoltageLimit")) {
                body.CurrentDemandReq.EVMaximumVoltageLimit_isUsed = true;
                body.CurrentDemandReq.EVMaximumVoltageLimit.Multiplier = inJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"].contains("Unit")) {
                    body.CurrentDemandReq.EVMaximumVoltageLimit.Unit_isUsed = true;
                    body.CurrentDemandReq.EVMaximumVoltageLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"]["Unit"].template get<int>());
                } else {
                    body.CurrentDemandReq.EVMaximumVoltageLimit.Unit_isUsed = false;
                }
                body.CurrentDemandReq.EVMaximumVoltageLimit.Value = inJson["Body"]["CurrentDemandReq"]["EVMaximumVoltageLimit"]["Value"].template get<uint16_t>();
            } else {
                body.CurrentDemandReq.EVMaximumVoltageLimit_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandReq"].contains("EVMaximumCurrentLimit")) {
                body.CurrentDemandReq.EVMaximumCurrentLimit_isUsed = true;
                body.CurrentDemandReq.EVMaximumCurrentLimit.Multiplier = inJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"].contains("Unit")) {
                    body.CurrentDemandReq.EVMaximumCurrentLimit.Unit_isUsed = true;
                    body.CurrentDemandReq.EVMaximumCurrentLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"]["Unit"].template get<int>());
                } else {
                    body.CurrentDemandReq.EVMaximumCurrentLimit.Unit_isUsed = false;
                }
                body.CurrentDemandReq.EVMaximumCurrentLimit.Value = inJson["Body"]["CurrentDemandReq"]["EVMaximumCurrentLimit"]["Value"].template get<uint16_t>();
            } else {
                body.CurrentDemandReq.EVMaximumCurrentLimit_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandReq"].contains("EVMaximumPowerLimit")) {
                body.CurrentDemandReq.EVMaximumPowerLimit_isUsed = true;
                body.CurrentDemandReq.EVMaximumPowerLimit.Multiplier = inJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"].contains("Unit")) {
                    body.CurrentDemandReq.EVMaximumPowerLimit.Unit_isUsed = true;
                    body.CurrentDemandReq.EVMaximumPowerLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"]["Unit"].template get<int>());
                } else {
                    body.CurrentDemandReq.EVMaximumPowerLimit.Unit_isUsed = false;
                }
                body.CurrentDemandReq.EVMaximumPowerLimit.Value = inJson["Body"]["CurrentDemandReq"]["EVMaximumPowerLimit"]["Value"].template get<uint16_t>();
            } else {
                body.CurrentDemandReq.EVMaximumPowerLimit_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandReq"].contains("BulkChargingComplete")) {
                body.CurrentDemandReq.BulkChargingComplete_isUsed = true;
                body.CurrentDemandReq.BulkChargingComplete = inJson["Body"]["CurrentDemandReq"]["BulkChargingComplete"].template get<int>();
            } else {
                body.CurrentDemandReq.BulkChargingComplete_isUsed = false;
            }

            body.CurrentDemandReq.ChargingComplete = inJson["Body"]["CurrentDemandReq"]["ChargingComplete"].template get<int>();

            if (inJson["Body"]["CurrentDemandReq"].contains("RemainingTimeToFullSoC")) {
                body.CurrentDemandReq.RemainingTimeToFullSoC_isUsed = true;
                body.CurrentDemandReq.RemainingTimeToFullSoC.Multiplier = inJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"].contains("Unit")) {
                    body.CurrentDemandReq.RemainingTimeToFullSoC.Unit_isUsed = true;
                    body.CurrentDemandReq.RemainingTimeToFullSoC.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"]["Unit"].template get<int>());
                } else {
                    body.CurrentDemandReq.RemainingTimeToFullSoC.Unit_isUsed = false;
                }
                body.CurrentDemandReq.RemainingTimeToFullSoC.Value = inJson["Body"]["CurrentDemandReq"]["RemainingTimeToFullSoC"]["Value"].template get<uint16_t>();
            } else {
                body.CurrentDemandReq.RemainingTimeToFullSoC_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandReq"].contains("RemainingTimeToBulkSoC")) {
                body.CurrentDemandReq.RemainingTimeToBulkSoC_isUsed = true;
                body.CurrentDemandReq.RemainingTimeToBulkSoC.Multiplier = inJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"].contains("Unit")) {
                    body.CurrentDemandReq.RemainingTimeToBulkSoC.Unit_isUsed = true;
                    body.CurrentDemandReq.RemainingTimeToBulkSoC.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"]["Unit"].template get<int>());
                } else {
                    body.CurrentDemandReq.RemainingTimeToBulkSoC.Unit_isUsed = false;
                }
                body.CurrentDemandReq.RemainingTimeToBulkSoC.Value = inJson["Body"]["CurrentDemandReq"]["RemainingTimeToBulkSoC"]["Value"].template get<uint16_t>();
            } else {
                body.CurrentDemandReq.RemainingTimeToBulkSoC_isUsed = false;
            }

            body.CurrentDemandReq.EVTargetVoltage.Multiplier = inJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"]["Multiplier"].template get<uint8_t>();
            if (inJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"].contains("Unit")) {
                body.CurrentDemandReq.EVTargetVoltage.Unit_isUsed = true;
                body.CurrentDemandReq.EVTargetVoltage.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"]["Unit"].template get<int>());
            } else {
                body.CurrentDemandReq.EVTargetVoltage.Unit_isUsed = false;
            }
            body.CurrentDemandReq.EVTargetVoltage.Value = inJson["Body"]["CurrentDemandReq"]["EVTargetVoltage"]["Value"].template get<uint16_t>();
        } else if (packetType == "CurrentDemandRes") {//////////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_CurrentDemandResType(&body.CurrentDemandRes);
            body.CurrentDemandRes_isUsed = true;

            body.CurrentDemandRes.ResponseCode = static_cast<din_responseCodeType>(inJson["Body"]["CurrentDemandRes"]["ResponseCode"].template get<int>());
            body.CurrentDemandRes.DC_EVSEStatus.EVSEStatusCode = static_cast<din_DC_EVSEStatusCodeType>(inJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());
            if (inJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                body.CurrentDemandRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                body.CurrentDemandRes.DC_EVSEStatus.EVSEIsolationStatus = static_cast<din_isolationLevelType>(inJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
            } else {
                body.CurrentDemandRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
            }
            body.CurrentDemandRes.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint32_t>();
            body.CurrentDemandRes.DC_EVSEStatus.EVSENotification = static_cast<din_EVSENotificationType>(inJson["Body"]["CurrentDemandRes"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());
        
            body.CurrentDemandRes.EVSEPresentVoltage.Multiplier = inJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"]["Multiplier"].template get<uint8_t>();
            if (inJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"].contains("Unit")) {
                body.CurrentDemandRes.EVSEPresentVoltage.Unit_isUsed = true;
                body.CurrentDemandRes.EVSEPresentVoltage.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"]["Unit"].template get<int>());
            } else {
                body.CurrentDemandRes.EVSEPresentVoltage.Unit_isUsed = false;
            }
            body.CurrentDemandRes.EVSEPresentVoltage.Value = inJson["Body"]["CurrentDemandRes"]["EVSEPresentVoltage"]["Value"].template get<uint16_t>();

            body.CurrentDemandRes.EVSEPresentCurrent.Multiplier = inJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"]["Multiplier"].template get<uint8_t>();
            if (inJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"].contains("Unit")) {
                body.CurrentDemandRes.EVSEPresentCurrent.Unit_isUsed = true;
                body.CurrentDemandRes.EVSEPresentCurrent.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"]["Unit"].template get<int>());
            } else {
                body.CurrentDemandRes.EVSEPresentCurrent.Unit_isUsed = false;
            }
            body.CurrentDemandRes.EVSEPresentCurrent.Value = inJson["Body"]["CurrentDemandRes"]["EVSEPresentCurrent"]["Value"].template get<uint16_t>();

            body.CurrentDemandRes.EVSECurrentLimitAchieved = inJson["Body"]["CurrentDemandRes"]["EVSECurrentLimitAchieved"].template get<int>();
            body.CurrentDemandRes.EVSEVoltageLimitAchieved = inJson["Body"]["CurrentDemandRes"]["EVSEVoltageLimitAchieved"].template get<int>();
            body.CurrentDemandRes.EVSEPowerLimitAchieved = inJson["Body"]["CurrentDemandRes"]["EVSEPowerLimitAchieved"].template get<int>();

            if (inJson["Body"]["CurrentDemandRes"].contains("EVSEMaximumVoltageLimit")) {
                body.CurrentDemandRes.EVSEMaximumVoltageLimit_isUsed = true;
                body.CurrentDemandRes.EVSEMaximumVoltageLimit.Multiplier = inJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"].contains("Unit")) {
                    body.CurrentDemandRes.EVSEMaximumVoltageLimit.Unit_isUsed = true;
                    body.CurrentDemandRes.EVSEMaximumVoltageLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"]["Unit"].template get<int>());
                } else {
                    body.CurrentDemandRes.EVSEMaximumVoltageLimit.Unit_isUsed = false;
                }
                body.CurrentDemandRes.EVSEMaximumVoltageLimit.Value = inJson["Body"]["CurrentDemandRes"]["EVSEMaximumVoltageLimit"]["Value"].template get<uint16_t>();
            } else {
                body.CurrentDemandRes.EVSEMaximumVoltageLimit_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandRes"].contains("EVSEMaximumCurrentLimit")) {
                body.CurrentDemandRes.EVSEMaximumCurrentLimit_isUsed = true;
                body.CurrentDemandRes.EVSEMaximumCurrentLimit.Multiplier = inJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"].contains("Unit")) {
                    body.CurrentDemandRes.EVSEMaximumCurrentLimit.Unit_isUsed = true;
                    body.CurrentDemandRes.EVSEMaximumCurrentLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"]["Unit"].template get<int>());
                } else {
                    body.CurrentDemandRes.EVSEMaximumCurrentLimit.Unit_isUsed = false;
                }
                body.CurrentDemandRes.EVSEMaximumCurrentLimit.Value = inJson["Body"]["CurrentDemandRes"]["EVSEMaximumCurrentLimit"]["Value"].template get<uint16_t>();
            } else {
                body.CurrentDemandRes.EVSEMaximumCurrentLimit_isUsed = false;
            }

            if (inJson["Body"]["CurrentDemandRes"].contains("EVSEMaximumPowerLimit")) {
                body.CurrentDemandRes.EVSEMaximumPowerLimit_isUsed = true;
                body.CurrentDemandRes.EVSEMaximumPowerLimit.Multiplier = inJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"]["Multiplier"].template get<uint8_t>();
                if (inJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"].contains("Unit")) {
                    body.CurrentDemandRes.EVSEMaximumPowerLimit.Unit_isUsed = true;
                    body.CurrentDemandRes.EVSEMaximumPowerLimit.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"]["Unit"].template get<int>());
                } else {
                    body.CurrentDemandRes.EVSEMaximumPowerLimit.Unit_isUsed = false;
                }
                body.CurrentDemandRes.EVSEMaximumPowerLimit.Value = inJson["Body"]["CurrentDemandRes"]["EVSEMaximumPowerLimit"]["Value"].template get<uint16_t>();
            } else {
                body.CurrentDemandRes.EVSEMaximumPowerLimit_isUsed = false;
            }
        } else if (packetType == "MeteringReceiptReq") {////////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "MeteringReceiptRes") {////////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "PaymentDetailsReq") {/////////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "PaymentDetailsRes") {/////////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "PowerDeliveryReq") {//////////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_PowerDeliveryReqType(&body.PowerDeliveryReq);
            body.PowerDeliveryReq_isUsed = true;

            body.PowerDeliveryReq.ReadyToChargeState = inJson["Body"]["PowerDeliveryReq"]["ReadyToChargeState"].template get<int>();

            if (inJson["Body"]["PowerDeliveryReq"].contains("ChargingProfile")) {
                body.PowerDeliveryReq.ChargingProfile_isUsed = true;
                body.PowerDeliveryReq.ChargingProfile.SAScheduleTupleID = inJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["SAScheduleTupleID"].template get<uint16_t>();
                body.PowerDeliveryReq.ChargingProfile.ProfileEntry.arrayLen = inJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["arrayLen"].template get<uint16_t>();
                for (int i=0; i<body.PowerDeliveryReq.ChargingProfile.ProfileEntry.arrayLen; i++){
                    body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryStart = inJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryStart"].template get<uint32_t>();
                    body.PowerDeliveryReq.ChargingProfile.ProfileEntry.array[i].ChargingProfileEntryMaxPower = inJson["Body"]["PowerDeliveryReq"]["ChargingProfile"]["ProfileEntry"]["array"][i]["ChargingProfileEntryMaxPower"].template get<uint16_t>();
                }
            } else {
                body.PowerDeliveryReq.ChargingProfile_isUsed = false;
            }

            if (inJson["Body"]["PowerDeliveryReq"].contains("DC_EVPowerDeliveryParameter")) {
                body.PowerDeliveryReq.DC_EVPowerDeliveryParameter_isUsed= true;
                body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVReady = inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVReady"].template get<int>();
                if (inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"].contains("EVCabinConditioning")) {
                    body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVCabinConditioning_isUsed = true;
                    body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVCabinConditioning = inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVCabinConditioning"].template get<int>();
                } else {
                    body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVCabinConditioning_isUsed = false;
                }
                if (inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"].contains("EVRESSConditioning")) {
                    body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVRESSConditioning_isUsed = true;
                    body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVRESSConditioning = inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVRESSConditioning"].template get<int>();
                } else {
                    body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVRESSConditioning_isUsed = false;
                }
                body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVErrorCode = static_cast<din_DC_EVErrorCodeType>(inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVErrorCode"].template get<int>());
                body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.DC_EVStatus.EVRESSSOC = inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["DC_EVStatus"]["EVRESSSOC"].template get<uint8_t>();

                if (inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"].contains("BulkChargingComplete")) {
                    body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.BulkChargingComplete_isUsed = true;
                    body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.BulkChargingComplete = inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["BulkChargingComplete"].template get<int>();
                } else {
                    body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.BulkChargingComplete_isUsed = false;
                }

                body.PowerDeliveryReq.DC_EVPowerDeliveryParameter.ChargingComplete = inJson["Body"]["PowerDeliveryReq"]["DC_EVPowerDeliveryParameter"]["ChargingComplete"].template get<int>();
            } else {
                body.PowerDeliveryReq.DC_EVPowerDeliveryParameter_isUsed = false;
            }

            if (inJson["Body"]["PowerDeliveryReq"].contains("EVPowerDeliveryParameter")) {
                body.PowerDeliveryReq.EVPowerDeliveryParameter_isUsed = true;
                body.PowerDeliveryReq.EVPowerDeliveryParameter._unused = inJson["Body"]["PowerDeliveryReq"]["EVPowerDeliveryParameter"]["_unused"].template get<int>();
            } else {
                body.PowerDeliveryReq.EVPowerDeliveryParameter_isUsed = false;
            }
        } else if (packetType == "PowerDeliveryRes") {//////////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_PowerDeliveryResType(&body.PowerDeliveryRes);
            body.PowerDeliveryRes_isUsed = true;

            body.PowerDeliveryRes.ResponseCode = static_cast<din_responseCodeType>(inJson["Body"]["PowerDeliveryRes"]["ResponseCode"].template get<int>());

            if (inJson["Body"]["PowerDeliveryRes"].contains("AC_EVSEStatus")) {
                body.PowerDeliveryRes.AC_EVSEStatus_isUsed = true;
                body.PowerDeliveryRes.AC_EVSEStatus.PowerSwitchClosed = inJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["PowerSwitchClosed"].template get<int>();
                body.PowerDeliveryRes.AC_EVSEStatus.RCD = inJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["RCD"].template get<int>();
                body.PowerDeliveryRes.AC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["NotificationMaxDelay"].template get<uint32_t>();
                body.PowerDeliveryRes.AC_EVSEStatus.EVSENotification = static_cast<din_EVSENotificationType>(inJson["Body"]["PowerDeliveryRes"]["AC_EVSEStatus"]["EVSENotification"].template get<int>());
                } else {
                body.PowerDeliveryRes.AC_EVSEStatus_isUsed = false;
            }

            if (inJson["Body"]["PowerDeliveryRes"].contains("DC_EVSEStatus")) {
                body.PowerDeliveryRes.DC_EVSEStatus_isUsed = true;
                if (inJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                    body.PowerDeliveryRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                    body.PowerDeliveryRes.DC_EVSEStatus.EVSEIsolationStatus = static_cast<din_isolationLevelType>(inJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
                } else {
                    body.PowerDeliveryRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
                }
                body.PowerDeliveryRes.DC_EVSEStatus.EVSEStatusCode = static_cast<din_DC_EVSEStatusCodeType>(inJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());
                body.PowerDeliveryRes.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint32_t>();
                body.PowerDeliveryRes.DC_EVSEStatus.EVSENotification = static_cast<din_EVSENotificationType>(inJson["Body"]["PowerDeliveryRes"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());
            } else {
                body.PowerDeliveryRes.DC_EVSEStatus_isUsed = false;
            }

            if (inJson["Body"]["PowerDeliveryRes"].contains("EVSEStatus")) {
                body.PowerDeliveryRes.EVSEStatus_isUsed = true;
                body.PowerDeliveryRes.EVSEStatus._unused = inJson["Body"]["PowerDeliveryRes"]["EVSEStatus"]["_unused"].template get<int>();
            } else {
                body.PowerDeliveryRes.EVSEStatus_isUsed = false;
            }
        } else if (packetType == "PreChargeReq") {//////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_PreChargeReqType(&body.PreChargeReq);
            body.PreChargeReq_isUsed = true;

            body.PreChargeReq.DC_EVStatus.EVReady = inJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVReady"].template get<int>();
            if (inJson["Body"]["PreChargeReq"]["DC_EVStatus"].contains("EVCabinConditioning")) {
                body.PreChargeReq.DC_EVStatus.EVCabinConditioning_isUsed = true;
                body.PreChargeReq.DC_EVStatus.EVCabinConditioning = inJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVCabinConditioning"].template get<int>();
            } else {
                body.PreChargeReq.DC_EVStatus.EVCabinConditioning_isUsed = false;
            }
            if (inJson["Body"]["PreChargeReq"]["DC_EVStatus"].contains("EVRESSConditioning")) {
                body.PreChargeReq.DC_EVStatus.EVRESSConditioning_isUsed = true;
                body.PreChargeReq.DC_EVStatus.EVRESSConditioning = inJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVRESSConditioning"].template get<int>();
            } else {
                body.PreChargeReq.DC_EVStatus.EVRESSConditioning_isUsed = false;
            }
            body.PreChargeReq.DC_EVStatus.EVErrorCode = static_cast<din_DC_EVErrorCodeType>(inJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVErrorCode"].template get<int>());
            body.PreChargeReq.DC_EVStatus.EVRESSSOC = inJson["Body"]["PreChargeReq"]["DC_EVStatus"]["EVRESSSOC"].template get<uint8_t>();

            body.PreChargeReq.EVTargetVoltage.Multiplier = inJson["Body"]["PreChargeReq"]["EVTargetVoltage"]["Multiplier"].template get<uint8_t>();
            if (inJson["Body"]["PreChargeReq"]["EVTargetVoltage"].contains("Unit")) {
                body.PreChargeReq.EVTargetVoltage.Unit_isUsed = true;
                body.PreChargeReq.EVTargetVoltage.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["PreChargeReq"]["EVTargetVoltage"]["Unit"].template get<int>());
            } else {
                body.PreChargeReq.EVTargetVoltage.Unit_isUsed = false;
            }
            body.PreChargeReq.EVTargetVoltage.Value = inJson["Body"]["PreChargeReq"]["EVTargetVoltage"]["Value"].template get<uint16_t>();

            body.PreChargeReq.EVTargetCurrent.Multiplier = inJson["Body"]["PreChargeReq"]["EVTargetCurrent"]["Multiplier"].template get<uint8_t>();
            if (inJson["Body"]["PreChargeReq"]["EVTargetCurrent"].contains("Unit")) {
                body.PreChargeReq.EVTargetCurrent.Unit_isUsed = true;
                body.PreChargeReq.EVTargetCurrent.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["PreChargeReq"]["EVTargetCurrent"]["Unit"].template get<int>());
            } else {
                body.PreChargeReq.EVTargetCurrent.Unit_isUsed = false;
            }
            body.PreChargeReq.EVTargetCurrent.Value = inJson["Body"]["PreChargeReq"]["EVTargetCurrent"]["Value"].template get<uint16_t>();
        } else if (packetType == "PreChargeRes") {//////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_PreChargeResType(&body.PreChargeRes);
            body.PreChargeRes_isUsed = true;

            body.PreChargeRes.ResponseCode = static_cast<din_responseCodeType>(inJson["Body"]["PreChargeRes"]["ResponseCode"].template get<int>());
            body.PreChargeRes.DC_EVSEStatus.EVSEStatusCode = static_cast<din_DC_EVSEStatusCodeType>(inJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["EVSEStatusCode"].template get<int>());
            if (inJson["Body"]["PreChargeRes"]["DC_EVSEStatus"].contains("EVSEIsolationStatus")) {
                body.PreChargeRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = true;
                body.PreChargeRes.DC_EVSEStatus.EVSEIsolationStatus = static_cast<din_isolationLevelType>(inJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["EVSEIsolationStatus"].template get<int>());
            } else {
                body.PreChargeRes.DC_EVSEStatus.EVSEIsolationStatus_isUsed = false;
            }
            body.PreChargeRes.DC_EVSEStatus.NotificationMaxDelay = inJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["NotificationMaxDelay"].template get<uint32_t>();
            body.PreChargeRes.DC_EVSEStatus.EVSENotification = static_cast<din_EVSENotificationType>(inJson["Body"]["PreChargeRes"]["DC_EVSEStatus"]["EVSENotification"].template get<int>());

            body.PreChargeRes.EVSEPresentVoltage.Multiplier = inJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"]["Multiplier"].template get<uint8_t>();
            if (inJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"].contains("Unit")) {
                body.PreChargeRes.EVSEPresentVoltage.Unit_isUsed = true;
                body.PreChargeRes.EVSEPresentVoltage.Unit = static_cast<din_unitSymbolType>(inJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"]["Unit"].template get<int>());
            } else {
                body.PreChargeRes.EVSEPresentVoltage.Unit_isUsed = false;
            }
            body.PreChargeRes.EVSEPresentVoltage.Value = inJson["Body"]["PreChargeRes"]["EVSEPresentVoltage"]["Value"].template get<uint16_t>();
        
        } else if (packetType == "ServiceDetailReq") {//////////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "ServiceDetailRes") {//////////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "ServiceDiscoveryReq") {///////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_ServiceDiscoveryReqType(&body.ServiceDiscoveryReq);
            body.ServiceDiscoveryReq_isUsed = true;

            if (inJson["Body"]["ServiceDiscoveryReq"].contains("ServiceScope")) {
                body.ServiceDiscoveryReq.ServiceScope_isUsed = true;
                body.ServiceDiscoveryReq.ServiceScope.charactersLen = inJson["Body"]["ServiceDiscoveryReq"]["ServiceScope"]["charactersLen"].template get<uint16_t>();
                for (int i=0; i<body.ServiceDiscoveryReq.ServiceScope.charactersLen; i++){
                    body.ServiceDiscoveryReq.ServiceScope.characters[i] = inJson["Body"]["ServiceDiscoveryReq"]["ServiceScope"]["characters"][i].template get<uint8_t>();
                }
            } else {
                body.ServiceDiscoveryReq.ServiceScope_isUsed = false;
            }

            if (inJson["Body"]["ServiceDiscoveryReq"].contains("ServiceCategory")) {
                body.ServiceDiscoveryReq.ServiceCategory_isUsed = true;
                body.ServiceDiscoveryReq.ServiceCategory = static_cast<din_serviceCategoryType>(inJson["Body"]["ServiceDiscoveryReq"]["ServiceCategory"].template get<int>());
            }
        } else if (packetType == "ServiceDiscoveryRes") {///////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_ServiceDiscoveryResType(&body.ServiceDiscoveryRes);
            body.ServiceDiscoveryRes_isUsed = true;

            body.ServiceDiscoveryRes.ResponseCode = static_cast<din_responseCodeType>(inJson["Body"]["ServiceDiscoveryRes"]["ResponseCode"].template get<int>());
            
            body.ServiceDiscoveryRes.PaymentOptions.PaymentOption.arrayLen = inJson["Body"]["ServiceDiscoveryRes"]["PaymentOptions"]["PaymentOption"]["arrayLen"].template get<uint16_t>();
            for (int i=0; i<body.ServiceDiscoveryRes.PaymentOptions.PaymentOption.arrayLen; i++){
                body.ServiceDiscoveryRes.PaymentOptions.PaymentOption.array[i] = static_cast<din_paymentOptionType>(inJson["Body"]["ServiceDiscoveryRes"]["PaymentOptions"]["PaymentOption"]["array"][i].template get<int>());
            }

            body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceID = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"]["ServiceID"].template get<uint16_t>();
            if (inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"].contains("ServiceName")) {
                body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceName_isUsed = true;
                body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceName.charactersLen = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"]["ServiceName"]["charactersLen"].template get<uint16_t>();
                for (int i=0; i<body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceName.charactersLen; i++){
                    body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceName.characters[i] = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"]["ServiceName"]["characters"][i].template get<uint8_t>();
                }
            } else {
                body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceName_isUsed = false;
            }
            body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceCategory = static_cast<din_serviceCategoryType>(inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"]["ServiceCategory"].template get<int>());
            if (inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"].contains("ServiceScope")) {
                body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceScope_isUsed = true;
                body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceScope.charactersLen = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"]["ServiceScope"]["charactersLen"].template get<uint16_t>();
                for (int i=0; i<body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceScope.charactersLen; i++){
                    body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceScope.characters[i] = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["ServiceTag"]["ServiceScope"]["characters"][i].template get<uint8_t>();
                }
            } else {
                body.ServiceDiscoveryRes.ChargeService.ServiceTag.ServiceScope_isUsed = false;
            }
            body.ServiceDiscoveryRes.ChargeService.FreeService = inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["FreeService"].template get<int>();
            body.ServiceDiscoveryRes.ChargeService.EnergyTransferType = static_cast<din_EVSESupportedEnergyTransferType>(inJson["Body"]["ServiceDiscoveryRes"]["ChargeService"]["EnergyTransferType"].template get<int>());

            if (inJson["Body"]["ServiceDiscoveryRes"].contains("ServiceList")) {
                body.ServiceDiscoveryRes.ServiceList_isUsed = true;
                body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceID = inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"]["ServiceID"].template get<uint16_t>();
                if (inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"].contains("ServiceName")) {
                    body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceName_isUsed = true;
                    body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceName.charactersLen = inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"]["ServiceName"]["charactersLen"].template get<uint16_t>();
                    for (int i=0; i<body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceName.charactersLen; i++){
                        body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceName.characters[i] = inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"]["ServiceName"]["characters"][i].template get<uint8_t>();
                    }
                } else {
                    body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceName_isUsed = false;
                }
                body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceCategory = static_cast<din_serviceCategoryType>(inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"]["ServiceCategory"].template get<int>());
                if (inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"].contains("ServiceScope")) {
                    body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceScope_isUsed = true;
                    body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceScope.charactersLen = inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"]["ServiceScope"]["charactersLen"].template get<uint16_t>();
                    for (int i=0; i<body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceScope.charactersLen; i++){
                        body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceScope.characters[i] = inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["ServiceTag"]["ServiceScope"]["characters"][i].template get<uint8_t>();
                    }
                } else {
                    body.ServiceDiscoveryRes.ServiceList.Service.ServiceTag.ServiceScope_isUsed = false;
                }
                body.ServiceDiscoveryRes.ServiceList.Service.FreeService = inJson["Body"]["ServiceDiscoveryRes"]["ServiceList"]["Service"]["FreeService"].template get<int>();
            } else {
                body.ServiceDiscoveryRes.ServiceList_isUsed = false;
            }
        } else if (packetType == "ServicePaymentSelectionReq") {////////////////////////////////////////////////////////////////////////////////////////////
            init_din_ServicePaymentSelectionReqType(&body.ServicePaymentSelectionReq);
            body.ServicePaymentSelectionReq_isUsed = true;

            body.ServicePaymentSelectionReq.SelectedPaymentOption = static_cast<din_paymentOptionType>(inJson["Body"]["ServicePaymentSelectionReq"]["SelectedPaymentOption"].template get<int>());

            body.ServicePaymentSelectionReq.SelectedServiceList.SelectedService.arrayLen = inJson["Body"]["ServicePaymentSelectionReq"]["SelectedServiceList"]["SelectedService"]["arrayLen"].template get<uint16_t>();
            for (int i=0; i<body.ServicePaymentSelectionReq.SelectedServiceList.SelectedService.arrayLen; i++){
                body.ServicePaymentSelectionReq.SelectedServiceList.SelectedService.array[i].ServiceID = inJson["Body"]["ServicePaymentSelectionReq"]["SelectedServiceList"]["SelectedService"]["array"][i]["ServiceID"].template get<uint16_t>();
                if (inJson["Body"]["ServicePaymentSelectionReq"]["SelectedServiceList"]["SelectedService"]["array"][i].contains("ParameterSetID")) {
                    body.ServicePaymentSelectionReq.SelectedServiceList.SelectedService.array[i].ParameterSetID_isUsed = true;
                    body.ServicePaymentSelectionReq.SelectedServiceList.SelectedService.array[i].ParameterSetID = inJson["Body"]["ServicePaymentSelectionReq"]["SelectedServiceList"]["SelectedService"]["array"][i]["ParameterSetID"].template get<uint16_t>();
                } else {
                    body.ServicePaymentSelectionReq.SelectedServiceList.SelectedService.array[i].ParameterSetID_isUsed = false;
                }
            }
        } else if (packetType == "ServicePaymentSelectionRes") {////////////////////////////////////////////////////////////////////////////////////////////
            init_din_ServicePaymentSelectionResType(&body.ServicePaymentSelectionRes);
            body.ServicePaymentSelectionRes_isUsed = true;

            body.ServicePaymentSelectionRes.ResponseCode = static_cast<din_responseCodeType>(inJson["Body"]["ServicePaymentSelectionRes"]["ResponseCode"].template get<int>());
        } else if (packetType == "SessionSetupReq") {///////////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_SessionSetupReqType(&body.SessionSetupReq);
            body.SessionSetupReq_isUsed = true;

            body.SessionSetupReq.EVCCID.bytesLen = inJson["Body"]["SessionSetupReq"]["EVCCID"]["bytesLen"].template get<uint16_t>();
            for (int i=0; i<body.SessionSetupReq.EVCCID.bytesLen; i++){
                body.SessionSetupReq.EVCCID.bytes[i] = inJson["Body"]["SessionSetupReq"]["EVCCID"]["bytes"][i].template get<uint8_t>();
            }
        } else if (packetType == "SessionSetupRes") {///////////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_SessionSetupResType(&body.SessionSetupRes);
            body.SessionSetupRes_isUsed = true;

            body.SessionSetupRes.ResponseCode = static_cast<din_responseCodeType>(inJson["Body"]["SessionSetupRes"]["ResponseCode"].template get<int>());

            body.SessionSetupRes.EVSEID.bytesLen = inJson["Body"]["SessionSetupRes"]["EVSEID"]["bytesLen"].template get<uint16_t>();
            for (int i=0; i<body.SessionSetupRes.EVSEID.bytesLen; i++){
                body.SessionSetupRes.EVSEID.bytes[i] = inJson["Body"]["SessionSetupRes"]["EVSEID"]["bytes"][i].template get<uint8_t>();
            }

            if (inJson["Body"]["SessionSetupRes"].contains("DateTimeNow")) {
                body.SessionSetupRes.DateTimeNow_isUsed = true;
                body.SessionSetupRes.DateTimeNow = inJson["Body"]["SessionSetupRes"]["DateTimeNow"].template get<uint64_t>();
            } else {
                body.SessionSetupRes.DateTimeNow_isUsed = false;
            }

        } else if (packetType == "SessionStopReq") {////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_SessionStopType(&body.SessionStopReq);
            body.SessionStopReq_isUsed = true;

            body.SessionStopReq._unused = inJson["Body"]["SessionStopReq"]["_unused"].template get<int>();
        } else if (packetType == "SessionStopRes") {////////////////////////////////////////////////////////////////////////////////////////////////////////
            init_din_SessionStopResType(&body.SessionStopRes);
            body.SessionStopRes_isUsed = true;

            body.SessionStopRes.ResponseCode = static_cast<din_responseCodeType>(inJson["Body"]["SessionStopRes"]["ResponseCode"].template get<int>());
        } else if (packetType == "WeldingDetectionReq") {///////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "WeldingDetectionRes") {///////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "WeldingDetectionReq") {///////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        } else if (packetType == "WeldingDetectionRes") {///////////////////////////////////////////////////////////////////////////////////////////////////
            return nullptr;
        }

        uint8_t* stream = new uint8_t[256];  // Dynamically allocate buffer
        exi_bitstream_t outEXI;
        size_t pos1 = 0;

        exi_bitstream_init(&outEXI, stream, 256, pos1, nullptr);
        encode_din_exiDocument(&outEXI, &inDoc);
        
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
    cxxopts::Options options("./DINProcessor --[encode/decode]='STRING_OPTIONAL' -i FILE_INPUT -o FILE_OUTPUT\n  ./DINProcessor --decode='809a021050908c0c0c0c0c51e000040080'\n  ./DINProcessor -e -i input.json -o output.txt\n  ./DINProcessor", "Encode and decode DIN EXI messages\nIf no arguement specified for input/outfile, will default to stdin/stdout\nMUST use double quotes (\") and lower case true/false");
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