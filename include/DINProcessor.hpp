#ifndef DIN_PROCESSOR_HPP
#define DIN_PROCESSOR_HPP

#include <nlohmann/json.hpp>
using json = nlohmann::json;

json getJson_CostType(const struct din_CostType& CostTypeDoc);
struct din_CostType getDoc_CostType(const json& CostTypeJson);

json getJson_RelativeTimeIntervalType(const struct din_RelativeTimeIntervalType& RelativeTimeIntervalTypeDoc);
struct din_RelativeTimeIntervalType getDoc_RelativeTimeIntervalType(const json& RelativeTimeIntervalTypeJson);

json getJson_IntervalType(const struct din_IntervalType& IntervalTypeDoc);
struct din_IntervalType getDoc_IntervalType(const json& IntervalTypeJson);

json getJson_ConsumptionCostType(const struct din_ConsumptionCostType& ConsumptionCostTypeDoc);
struct din_ConsumptionCostType getDoc_ConsumptionCostType(const json& ConsumptionCostTypeJson);

json getJson_TransformType(const struct din_TransformType& TransformTypeDoc);
struct din_TransformType getDoc_TransformType(const json& TransformTypeJson);

json getJson_PMaxScheduleEntryType(const struct din_PMaxScheduleEntryType& PMaxScheduleEntryTypeDoc);
struct din_PMaxScheduleEntryType getDoc_PMaxScheduleEntryType(const json& PMaxScheduleEntryTypeJson);

json getJson_SalesTariffEntryType(const struct din_SalesTariffEntryType& SalesTariffEntryTypeDoc);
struct din_SalesTariffEntryType getDoc_SalesTariffEntryType(const json& SalesTariffEntryTypeJson);

json getJson_TransformsType(const struct din_TransformsType& TransformsTypeDoc);
struct din_TransformsType getDoc_TransformsType(const json& TransformsTypeJson);

json getJson_DSAKeyValueType(const struct din_DSAKeyValueType& DSAKeyValueTypeDoc);
struct din_DSAKeyValueType getDoc_DSAKeyValueType(const json& DSAKeyValueTypeJson);

json getJson_X509IssuerSerialType(const struct din_X509IssuerSerialType& X509IssuerSerialTypeDoc);
struct din_X509IssuerSerialType getDoc_X509IssuerSerialType(const json& X509IssuerSerialTypeJson);

json getJson_DigestMethodType(const struct din_DigestMethodType& DigestMethodTypeDoc);
struct din_DigestMethodType getDoc_DigestMethodType(const json& DigestMethodTypeJson);

json getJson_RSAKeyValueType(const struct din_RSAKeyValueType& RSAKeyValueTypeDoc);
struct din_RSAKeyValueType getDoc_RSAKeyValueType(const json& RSAKeyValueTypeJson);

json getJson_PMaxScheduleType(const struct din_PMaxScheduleType& PMaxScheduleTypeDoc);
struct din_PMaxScheduleType getDoc_PMaxScheduleType(const json& PMaxScheduleTypeJson);

json getJson_SalesTariffType(const struct din_SalesTariffType& SalesTariffTypeDoc);
struct din_SalesTariffType getDoc_SalesTariffType(const json& SalesTariffTypeJson);

json getJson_CanonicalizationMethodType(const struct din_CanonicalizationMethodType& CanonicalizationMethodTypeDoc);
struct din_CanonicalizationMethodType getDoc_CanonicalizationMethodType(const json& CanonicalizationMethodTypeJson);

json getJson_ServiceTagType(const struct din_ServiceTagType& ServiceTagTypeDoc);
struct din_ServiceTagType getDoc_ServiceTagType(const json& ServiceTagTypeJson);

json getJson_ServiceType(const struct din_ServiceType& ServiceTypeDoc);
struct din_ServiceType getDoc_ServiceType(const json& ServiceTypeJson);

json getJson_SelectedServiceType(const struct din_SelectedServiceType& SelectedServiceTypeDoc);
struct din_SelectedServiceType getDoc_SelectedServiceType(const json& SelectedServiceTypeJson);

json getJson_SAScheduleTupleType(const struct din_SAScheduleTupleType& SAScheduleTupleTypeDoc);
struct din_SAScheduleTupleType getDoc_SAScheduleTupleType(const json& SAScheduleTupleTypeJson);

json getJson_AC_EVSEStatusType(const struct din_AC_EVSEStatusType& AC_EVSEStatusTypeDoc);
struct din_AC_EVSEStatusType getDoc_AC_EVSEStatusType(const json& AC_EVSEStatusTypeJson);

json getJson_SignatureMethodType(const struct din_SignatureMethodType& SignatureMethodTypeDoc);
struct din_SignatureMethodType getDoc_SignatureMethodType(const json& SignatureMethodTypeJson);

json getJson_KeyValueType(const struct din_KeyValueType& KeyValueTypeDoc);
struct din_KeyValueType getDoc_KeyValueType(const json& KeyValueTypeJson);

json getJson_SubCertificatesType(const struct din_SubCertificatesType& SubCertificatesTypeDoc);
struct din_SubCertificatesType getDoc_SubCertificatesType(const json& SubCertificatesTypeJson);

json getJson_ProfileEntryType(const struct din_ProfileEntryType& ProfileEntryTypeDoc);
struct din_ProfileEntryType getDoc_ProfileEntryType(const json& ProfileEntryTypeJson);

json getJson_ReferenceType(const struct din_ReferenceType& ReferenceTypeDoc);
struct din_ReferenceType getDoc_ReferenceType(const json& ReferenceTypeJson);

json getJson_RetrievalMethodType(const struct din_RetrievalMethodType& RetrievalMethodTypeDoc);
struct din_RetrievalMethodType getDoc_RetrievalMethodType(const json& RetrievalMethodTypeJson);

json getJson_X509DataType(const struct din_X509DataType& X509DataTypeDoc);
struct din_X509DataType getDoc_X509DataType(const json& X509DataTypeJson);

json getJson_PGPDataType(const struct din_PGPDataType& PGPDataTypeDoc);
struct din_PGPDataType getDoc_PGPDataType(const json& PGPDataTypeJson);

json getJson_SPKIDataType(const struct din_SPKIDataType& SPKIDataTypeDoc);
struct din_SPKIDataType getDoc_SPKIDataType(const json& SPKIDataTypeJson);

json getJson_SignedInfoType(const struct din_SignedInfoType& SignedInfoTypeDoc);
struct din_SignedInfoType getDoc_SignedInfoType(const json& SignedInfoTypeJson);

json getJson_DC_EVStatusType(const struct din_DC_EVStatusType& DC_EVStatusTypeDoc);
struct din_DC_EVStatusType getDoc_DC_EVStatusType(const json& DC_EVStatusTypeJson);

json getJson_SignatureValueType(const struct din_SignatureValueType& SignatureValueTypeDoc);
struct din_SignatureValueType getDoc_SignatureValueType(const json& SignatureValueTypeJson);

json getJson_CertificateChainType(const struct din_CertificateChainType& CertificateChainTypeDoc);
struct din_CertificateChainType getDoc_CertificateChainType(const json& CertificateChainTypeJson);

json getJson_DC_EVSEStatusType(const struct din_DC_EVSEStatusType& DC_EVSEStatusTypeDoc);
struct din_DC_EVSEStatusType getDoc_DC_EVSEStatusType(const json& DC_EVSEStatusTypeJson);

json getJson_PhysicalValueType(const struct din_PhysicalValueType& PhysicalValueTypeDoc);
struct din_PhysicalValueType getDoc_PhysicalValueType(const json& PhysicalValueTypeJson);

json getJson_ParameterType(const struct din_ParameterType& ParameterTypeDoc);
struct din_ParameterType getDoc_ParameterType(const json& ParameterTypeJson);

json getJson_ParameterSetType(const struct din_ParameterSetType& ParameterSetTypeDoc);
struct din_ParameterSetType getDoc_ParameterSetType(const json& ParameterSetTypeJson);

json getJson_ListOfRootCertificateIDsType(const struct din_ListOfRootCertificateIDsType& ListOfRootCertificateIDsTypeDoc);
struct din_ListOfRootCertificateIDsType getDoc_ListOfRootCertificateIDsType(const json& ListOfRootCertificateIDsTypeJson);

json getJson_PaymentOptionsType(const struct din_PaymentOptionsType& PaymentOptionsTypeDoc);
struct din_PaymentOptionsType getDoc_PaymentOptionsType(const json& PaymentOptionsTypeJson);

json getJson_SelectedServiceListType(const struct din_SelectedServiceListType& SelectedServiceListTypeDoc);
struct din_SelectedServiceListType getDoc_SelectedServiceListType(const json& SelectedServiceListTypeJson);

json getJson_AC_EVChargeParameterType(const struct din_AC_EVChargeParameterType& AC_EVChargeParameterTypeDoc);
struct din_AC_EVChargeParameterType getDoc_AC_EVChargeParameterType(const json& AC_EVChargeParameterTypeJson);

json getJson_DC_EVChargeParameterType(const struct din_DC_EVChargeParameterType& DC_EVChargeParameterTypeDoc);
struct din_DC_EVChargeParameterType getDoc_DC_EVChargeParameterType(const json& DC_EVChargeParameterTypeJson);

json getJson_EVChargeParameterType(const struct din_EVChargeParameterType& EVChargeParameterTypeDoc);
struct din_EVChargeParameterType getDoc_EVChargeParameterType(const json& EVChargeParameterTypeJson);

json getJson_ChargingProfileType(const struct din_ChargingProfileType& ChargingProfileTypeDoc);
struct din_ChargingProfileType getDoc_ChargingProfileType(const json& ChargingProfileTypeJson);

json getJson_EVSEStatusType(const struct din_EVSEStatusType& EVSEStatusTypeDoc);
struct din_EVSEStatusType getDoc_EVSEStatusType(const json& EVSEStatusTypeJson);

json getJson_KeyInfoType(const struct din_KeyInfoType& KeyInfoTypeDoc);
struct din_KeyInfoType getDoc_KeyInfoType(const json& KeyInfoTypeJson);

json getJson_ServiceChargeType(const struct din_ServiceChargeType& ServiceChargeTypeDoc);
struct din_ServiceChargeType getDoc_ServiceChargeType(const json& ServiceChargeTypeJson);

json getJson_ServiceParameterListType(const struct din_ServiceParameterListType& ServiceParameterListTypeDoc);
struct din_ServiceParameterListType getDoc_ServiceParameterListType(const json& ServiceParameterListTypeJson);

json getJson_SAScheduleListType(const struct din_SAScheduleListType& SAScheduleListTypeDoc);
struct din_SAScheduleListType getDoc_SAScheduleListType(const json& SAScheduleListTypeJson);

json getJson_SASchedulesType(const struct din_SASchedulesType& SASchedulesTypeDoc);
struct din_SASchedulesType getDoc_SASchedulesType(const json& SASchedulesTypeJson);

json getJson_DC_EVPowerDeliveryParameterType(const struct din_DC_EVPowerDeliveryParameterType& DC_EVPowerDeliveryParameterTypeDoc);
struct din_DC_EVPowerDeliveryParameterType getDoc_DC_EVPowerDeliveryParameterType(const json& DC_EVPowerDeliveryParameterTypeJson);

json getJson_EVPowerDeliveryParameterType(const struct din_EVPowerDeliveryParameterType& EVPowerDeliveryParameterTypeDoc);
struct din_EVPowerDeliveryParameterType getDoc_EVPowerDeliveryParameterType(const json& EVPowerDeliveryParameterTypeJson);

json getJson_ObjectType(const struct din_ObjectType& ObjectTypeDoc);
struct din_ObjectType getDoc_ObjectType(const json& ObjectTypeJson);

json getJson_ServiceTagListType(const struct din_ServiceTagListType& ServiceTagListTypeDoc);
struct din_ServiceTagListType getDoc_ServiceTagListType(const json& ServiceTagListTypeJson);

json getJson_DC_EVSEChargeParameterType(const struct din_DC_EVSEChargeParameterType& DC_EVSEChargeParameterTypeDoc);
struct din_DC_EVSEChargeParameterType getDoc_DC_EVSEChargeParameterType(const json& DC_EVSEChargeParameterTypeJson);

json getJson_AC_EVSEChargeParameterType(const struct din_AC_EVSEChargeParameterType& AC_EVSEChargeParameterTypeDoc);
struct din_AC_EVSEChargeParameterType getDoc_AC_EVSEChargeParameterType(const json& AC_EVSEChargeParameterTypeJson);

json getJson_EVSEChargeParameterType(const struct din_EVSEChargeParameterType& EVSEChargeParameterTypeDoc);
struct din_EVSEChargeParameterType getDoc_EVSEChargeParameterType(const json& EVSEChargeParameterTypeJson);

json getJson_MeterInfoType(const struct din_MeterInfoType& MeterInfoTypeDoc);
struct din_MeterInfoType getDoc_MeterInfoType(const json& MeterInfoTypeJson);

json getJson_CertificateInstallationResType(const struct din_CertificateInstallationResType& CertificateInstallationResTypeDoc);
struct din_CertificateInstallationResType getDoc_CertificateInstallationResType(const json& CertificateInstallationResTypeJson);

json getJson_CableCheckReqType(const struct din_CableCheckReqType& CableCheckReqTypeDoc);
struct din_CableCheckReqType getDoc_CableCheckReqType(const json& CableCheckReqTypeJson);

json getJson_CableCheckResType(const struct din_CableCheckResType& CableCheckResTypeDoc);
struct din_CableCheckResType getDoc_CableCheckResType(const json& CableCheckResTypeJson);

json getJson_PreChargeReqType(const struct din_PreChargeReqType& PreChargeReqTypeDoc);
struct din_PreChargeReqType getDoc_PreChargeReqType(const json& PreChargeReqTypeJson);

json getJson_PreChargeResType(const struct din_PreChargeResType& PreChargeResTypeDoc);
struct din_PreChargeResType getDoc_PreChargeResType(const json& PreChargeResTypeJson);

json getJson_CurrentDemandReqType(const struct din_CurrentDemandReqType& CurrentDemandReqTypeDoc);
struct din_CurrentDemandReqType getDoc_CurrentDemandReqType(const json& CurrentDemandReqTypeJson);

json getJson_CurrentDemandResType(const struct din_CurrentDemandResType& CurrentDemandResTypeDoc);
struct din_CurrentDemandResType getDoc_CurrentDemandResType(const json& CurrentDemandResTypeJson);

json getJson_WeldingDetectionReqType(const struct din_WeldingDetectionReqType& WeldingDetectionReqTypeDoc);
struct din_WeldingDetectionReqType getDoc_WeldingDetectionReqType(const json& WeldingDetectionReqTypeJson);

json getJson_WeldingDetectionResType(const struct din_WeldingDetectionResType& WeldingDetectionResTypeDoc);
struct din_WeldingDetectionResType getDoc_WeldingDetectionResType(const json& WeldingDetectionResTypeJson);

json getJson_SessionSetupReqType(const struct din_SessionSetupReqType& SessionSetupReqTypeDoc);
struct din_SessionSetupReqType getDoc_SessionSetupReqType(const json& SessionSetupReqTypeJson);

json getJson_CertificateInstallationReqType(const struct din_CertificateInstallationReqType& CertificateInstallationReqTypeDoc);
struct din_CertificateInstallationReqType getDoc_CertificateInstallationReqType(const json& CertificateInstallationReqTypeJson);

json getJson_SessionSetupResType(const struct din_SessionSetupResType& SessionSetupResTypeDoc);
struct din_SessionSetupResType getDoc_SessionSetupResType(const json& SessionSetupResTypeJson);

json getJson_ServiceDiscoveryReqType(const struct din_ServiceDiscoveryReqType& ServiceDiscoveryReqTypeDoc);
struct din_ServiceDiscoveryReqType getDoc_ServiceDiscoveryReqType(const json& ServiceDiscoveryReqTypeJson);

json getJson_ServiceDiscoveryResType(const struct din_ServiceDiscoveryResType& ServiceDiscoveryResTypeDoc);
struct din_ServiceDiscoveryResType getDoc_ServiceDiscoveryResType(const json& ServiceDiscoveryResTypeJson);

json getJson_ServiceDetailReqType(const struct din_ServiceDetailReqType& ServiceDetailReqTypeDoc);
struct din_ServiceDetailReqType getDoc_ServiceDetailReqType(const json& ServiceDetailReqTypeJson);

json getJson_ServiceDetailResType(const struct din_ServiceDetailResType& ServiceDetailResTypeDoc);
struct din_ServiceDetailResType getDoc_ServiceDetailResType(const json& ServiceDetailResTypeJson);

json getJson_ServicePaymentSelectionReqType(const struct din_ServicePaymentSelectionReqType& ServicePaymentSelectionReqTypeDoc);
struct din_ServicePaymentSelectionReqType getDoc_ServicePaymentSelectionReqType(const json& ServicePaymentSelectionReqTypeJson);

json getJson_ServicePaymentSelectionResType(const struct din_ServicePaymentSelectionResType& ServicePaymentSelectionResTypeDoc);
struct din_ServicePaymentSelectionResType getDoc_ServicePaymentSelectionResType(const json& ServicePaymentSelectionResTypeJson);

json getJson_PaymentDetailsReqType(const struct din_PaymentDetailsReqType& PaymentDetailsReqTypeDoc);
struct din_PaymentDetailsReqType getDoc_PaymentDetailsReqType(const json& PaymentDetailsReqTypeJson);

json getJson_PaymentDetailsResType(const struct din_PaymentDetailsResType& PaymentDetailsResTypeDoc);
struct din_PaymentDetailsResType getDoc_PaymentDetailsResType(const json& PaymentDetailsResTypeJson);

json getJson_ContractAuthenticationReqType(const struct din_ContractAuthenticationReqType& ContractAuthenticationReqTypeDoc);
struct din_ContractAuthenticationReqType getDoc_ContractAuthenticationReqType(const json& ContractAuthenticationReqTypeJson);

json getJson_ContractAuthenticationResType(const struct din_ContractAuthenticationResType& ContractAuthenticationResTypeDoc);
struct din_ContractAuthenticationResType getDoc_ContractAuthenticationResType(const json& ContractAuthenticationResTypeJson);

json getJson_ChargeParameterDiscoveryReqType(const struct din_ChargeParameterDiscoveryReqType& ChargeParameterDiscoveryReqTypeDoc);
struct din_ChargeParameterDiscoveryReqType getDoc_ChargeParameterDiscoveryReqType(const json& ChargeParameterDiscoveryReqTypeJson);

json getJson_ChargeParameterDiscoveryResType(const struct din_ChargeParameterDiscoveryResType& ChargeParameterDiscoveryResTypeDoc);
struct din_ChargeParameterDiscoveryResType getDoc_ChargeParameterDiscoveryResType(const json& ChargeParameterDiscoveryResTypeJson);

json getJson_PowerDeliveryReqType(const struct din_PowerDeliveryReqType& PowerDeliveryReqTypeDoc);
struct din_PowerDeliveryReqType getDoc_PowerDeliveryReqType(const json& PowerDeliveryReqTypeJson);

json getJson_PowerDeliveryResType(const struct din_PowerDeliveryResType& PowerDeliveryResTypeDoc);
struct din_PowerDeliveryResType getDoc_PowerDeliveryResType(const json& PowerDeliveryResTypeJson);

json getJson_ChargingStatusReqType(const struct din_ChargingStatusReqType& ChargingStatusReqTypeDoc);
struct din_ChargingStatusReqType getDoc_ChargingStatusReqType(const json& ChargingStatusReqTypeJson);

json getJson_ChargingStatusResType(const struct din_ChargingStatusResType& ChargingStatusResTypeDoc);
struct din_ChargingStatusResType getDoc_ChargingStatusResType(const json& ChargingStatusResTypeJson);

json getJson_MeteringReceiptReqType(const struct din_MeteringReceiptReqType& MeteringReceiptReqTypeDoc);
struct din_MeteringReceiptReqType getDoc_MeteringReceiptReqType(const json& MeteringReceiptReqTypeJson);

json getJson_MeteringReceiptResType(const struct din_MeteringReceiptResType& MeteringReceiptResTypeDoc);
struct din_MeteringReceiptResType getDoc_MeteringReceiptResType(const json& MeteringReceiptResTypeJson);

json getJson_SessionStopType(const struct din_SessionStopType& SessionStopTypeDoc);
struct din_SessionStopType getDoc_SessionStopType(const json& SessionStopTypeJson);

json getJson_SessionStopResType(const struct din_SessionStopResType& SessionStopResTypeDoc);
struct din_SessionStopResType getDoc_SessionStopResType(const json& SessionStopResTypeJson);

json getJson_CertificateUpdateReqType(const struct din_CertificateUpdateReqType& CertificateUpdateReqTypeDoc);
struct din_CertificateUpdateReqType getDoc_CertificateUpdateReqType(const json& CertificateUpdateReqTypeJson);

json getJson_CertificateUpdateResType(const struct din_CertificateUpdateResType& CertificateUpdateResTypeDoc);
struct din_CertificateUpdateResType getDoc_CertificateUpdateResType(const json& CertificateUpdateResTypeJson);

json getJson_BodyBaseType(const struct din_BodyBaseType& BodyBaseTypeDoc);
struct din_BodyBaseType getDoc_BodyBaseType(const json& BodyBaseTypeJson);

json getJson_NotificationType(const struct din_NotificationType& NotificationTypeDoc);
struct din_NotificationType getDoc_NotificationType(const json& NotificationTypeJson);

json getJson_SignatureType(const struct din_SignatureType& SignatureTypeDoc);
struct din_SignatureType getDoc_SignatureType(const json& SignatureTypeJson);

json getJson_MessageHeaderType(const struct din_MessageHeaderType& MessageHeaderTypeDoc);
struct din_MessageHeaderType getDoc_MessageHeaderType(const json& MessageHeaderTypeJson);

json getJson_BodyType(const struct din_BodyType& BodyTypeDoc);
struct din_BodyType getDoc_BodyType(const json& BodyTypeJson);

json getJson_V2G_Message(const struct din_V2G_Message& V2G_MessageDoc);
struct din_V2G_Message getDoc_V2G_Message(const json& V2G_MessageJson);

json getJson_exiDocument(const struct din_exiDocument& exiDocumentDoc);
struct din_exiDocument getDoc_exiDocument(const json& exiDocumentJson);

struct encoded_data {
    uint8_t* buffer;
    size_t size;
};

const char* decodeFromPythonBytes(const uint8_t* data, size_t size);
encoded_data* encodeFromPythonDict(const char* inString);
void free_encoded_data(encoded_data* data);

#endif // DIN_PROCESSOR_HPP