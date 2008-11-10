--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097
-- 
-- 
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License published by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
--  
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--  
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
--  
--  Report problems and direct all questions to:
--  
--      Rex McMaster, rmcm@compsoft.com.au
--  
-- 



-- Create VIew "hicv" based on "mdbt"
--

-- \i schema/DBNAME/hicv-functions.sql   -- Functions need to be defined before the VIEW
--
-- To create this from HICE
-- psql -q -A -t -d prac_43R4 -c "select hice_name from hice where hice_input = 'Y' order by hice_name" 
--     | awk '{printf("  ::text            as hicv_%-50s-- %s\n",tolower($1)",",$1)}'

drop view hicv;
create view hicv                         -- HIC-Online interface
  as select

--
-- HIC Elements
-- 

  (case when invc_balance(svpf_invc__sequence) = 0.0 then 'Y'
    else 'N' end)                       as hicv_accountpaidind,                 -- AccountPaidInd
  ''::text                              as hicv_accountreferencenum,            -- AccountReferenceNum
  ''::text                              as hicv_admissiondate,                  -- AdmissionDate
  ''::text                              as hicv_aftercareoverrideind,           -- AfterCareOverrideInd
  ''::text                              as hicv_approvalnum,                    -- ApprovalNum
  ''::text                              as hicv_associatename,                  -- AssociateName
  ''::text                              as hicv_atsiindicator,                  -- AtsiIndicator
  ''::text                              as hicv_authproxyname,                  -- AuthProxyName
  ''::text                              as hicv_authproxypasswd,                -- AuthProxyPasswd
  ''::text                              as hicv_authproxypasswd_proxyname,      -- AuthProxyPasswd.<proxyname>
  ''::text                              as hicv_authproxyuserid,                -- AuthProxyUserId
  ''::text                              as hicv_authproxyuserid_proxyname,      -- AuthProxyUserId.<proxyname>
  ''::text                              as hicv_authorityapprovalnum,           -- AuthorityApprovalNum
  ''::text                              as hicv_authorityprescriptionnum,       -- AuthorityPrescriptionNum
  ''::text                              as hicv_accountreferenceid,             -- AccountReferenceId
  ''::text                              as hicv_bankaccountname,                -- BankAccountName
  ''::text                              as hicv_bankaccountnum,                 -- BankAccountNum
  ''::text                              as hicv_bsbcode,                        -- BSBCode
  mdaf_benefit_authorised               as hicv_benefitassignmentauthorised,    -- BenefitAssignmentAuthorised
  ''::text                              as hicv_billingagentid,                 -- BillingAgentId
  ''::text                              as hicv_buffersize,                     -- BufferSize
  (svpf_amount*100)::integer            as hicv_chargeamount,                   -- ChargeAmount
  ''::text                              as hicv_claimid,                        -- ClaimId
  ''::text                              as hicv_claimperiodnum,                 -- ClaimPeriodNum
  ''::text                              as hicv_claimreference,                 -- ClaimReference
  mdbt_claimsubauth                     as hicv_claimsubmissionauthorised,      -- ClaimSubmissionAuthorised
  ''::text                              as hicv_claimtypecde,                   -- ClaimTypeCde
  ''::text                              as hicv_claimantaddressline1,           -- ClaimantAddressLine1
  ''::text                              as hicv_claimantaddressline2,           -- ClaimantAddressLine2
  ''::text                              as hicv_claimantaddresslocality,        -- ClaimantAddressLocality
  ''::text                              as hicv_claimantaddresspostcode,        -- ClaimantAddressPostcode
  ''::text                              as hicv_claimantdateofbirth,            -- ClaimantDateOfBirth
  ''::text                              as hicv_claimantfamilyname,             -- ClaimantFamilyName
  ''::text                              as hicv_claimantfirstname,              -- ClaimantFirstName
  ''::text                              as hicv_claimantmedicarecardnum,        -- ClaimantMedicareCardNum
  ''::text                              as hicv_claimantreferencenum,           -- ClaimantReferenceNum
  ''::text                              as hicv_cliniccode,                     -- ClinicCode
  ''::text                              as hicv_communitycode,                  -- CommunityCode
  ''::text                              as hicv_compensationclaimind,           -- CompensationClaimInd
  ''::text                              as hicv_contactphonenum,                -- ContactPhoneNum
  ''::text                              as hicv_contenttype,                    -- ContentType
  ''::text                              as hicv_dateofdispensing,               -- DateOfDispensing
  ''::text                              as hicv_dateofimmunisation,             -- DateOfImmunisation
  to_char(mdbt_claim_date, 'DDMMYYYY')  as hicv_dateoflodgement,                -- DateOfLodgement
  ''::text                              as hicv_dateofprescribing,              -- DateOfPrescribing
  to_char(svpf_date_service,'DDMMYYYY') as hicv_dateofservice,                  -- DateOfService
  ''::text                              as hicv_dateofsupply,                   -- DateOfSupply
  ''::text                              as hicv_dateoftransmission,             -- DateOfTransmission
  ''::text                              as hicv_dischargedate,                  -- DischargeDate
  ''::text                              as hicv_distancekms,                    -- DistanceKms
  'N'::text                             as hicv_duplicateserviceoverrideind,    -- DuplicateServiceOverrideInd
  ''::text                              as hicv_entitlementid,                  -- EntitlementId
  ''::text                              as hicv_equipmentid,                    -- EquipmentId
  ''::text                              as hicv_facilityid,                     -- FacilityId
  patn_psnam                            as hicv_familyname,                     -- FamilyName
  ''::text                              as hicv_fieldquantity,                  -- FieldQuantity
  ''::text                              as hicv_filepath,                       -- FilePath
  ''::text                              as hicv_financialinterestdisclosureind, -- FinancialInterestDisclosureInd
  ''::text                              as hicv_formcategory,                   -- FormCategory
  ''::text                              as hicv_fundbrandid,                    -- FundBrandId
  ''::text                              as hicv_fundpayeeid,                    -- FundPayeeId
  ''::text                              as hicv_givenname,                      -- GivenName
  ''::text                              as hicv_glassbottleind,                 -- GlassBottleInd
  ''::text                              as hicv_hclpassphrase,                  -- HclPassphrase
  ''::text                              as hicv_hepbbirthdosedate,              -- HepBBirthDoseDate
  ''::text                              as hicv_hepbbirthdoseind,               -- HepBBirthDoseInd
  ''::text                              as hicv_hospitalind,                    -- HospitalInd
  ''::text                              as hicv_hospitalprovidernum,            -- HospitalProviderNum
  ''::text                              as hicv_ifcissuecde,                    -- IFCIssueCde
  ''::text                              as hicv_immediatesupplynecessaryind,    -- ImmediateSupplyNecessaryInd
  ''::text                              as hicv_immunisingprovidernum,          -- ImmunisingProviderNum
  ''::text                              as hicv_informationprovidernum,         -- InformationProviderNum
  svpf_serv_code                        as hicv_itemnum,                        -- ItemNum
  ''::text                              as hicv_lccid,                          -- LCCId
  mdbt_location_id                      as hicv_locationid,                     -- LocationId
  ''::text                              as hicv_logicpackdir,                   -- LogicPackDir
  ''::text                              as hicv_lspnum,                         -- LSPNum
  ''::text                              as hicv_medicarenum,                    -- MedicareNum
  ''::text                              as hicv_multipleprocedureoverrideind,   -- MultipleProcedureOverrideInd
  ''::text                              as hicv_nextduedate,                    -- NextDueDate
  ''::text                              as hicv_noofpatientsseen,               -- NoOfPatientsSeen
  ''::text                              as hicv_numberofrepeats,                -- NumberOfRepeats
  ''::text                              as hicv_numberofservices,               -- NumberOfServices
  ''::text                              as hicv_opvtypecde,                     -- OPVTypeCde
  ''::text                              as hicv_pbsitemcode,                    -- PBSItemCode
  ''::text                              as hicv_pbsreferencenum,                -- PBSReferenceNum
  ''::text                              as hicv_participanttype,                -- ParticipantType
  ''::text                              as hicv_patientaddressline1,            -- PatientAddressLine1
  ''::text                              as hicv_patientaddressline2,            -- PatientAddressLine2
  ''::text                              as hicv_patientaddresslocality,         -- PatientAddressLocality
  ''::text                              as hicv_patientaddresspostcode,         -- PatientAddressPostcode
  ''::text                              as hicv_patientaliasfamilyname,         -- PatientAliasFamilyName
  ''::text                              as hicv_patientaliasfirstname,          -- PatientAliasFirstName
  ''::text                              as hicv_patientcategory,                -- PatientCategory
  ''::text                              as hicv_patientcontribamt,              -- PatientContribAmt
  to_char(patn_dob, 'DDMMYYYY')         as hicv_patientdateofbirth,             -- PatientDateOfBirth
  patn_psnam                            as hicv_patientfamilyname,              -- PatientFamilyName
  patn_fsnam                            as hicv_patientfirstname,               -- PatientFirstName
  ''::text                              as hicv_patientfundmembershipnum,       -- PatientFundMembershipNum
  ''::text                              as hicv_patientfundupi,                 -- PatientFundUPI
  ''::text                              as hicv_patientgender,                  -- PatientGender
  substring(patn_medicare from '^[0-9]*')
                                        as hicv_patientmedicarecardnum,         -- PatientMedicareCardNum
  ''::text                              as hicv_patientproviderchildid,         -- PatientProviderChildId
  (case when length(patn_medicare) > 0 then
          substring(coalesce(substring(patn_medicare from '/[0-9]+'), '1') from '[0-9]+')
   else '' end)
                                        as hicv_patientreferencenum,            -- PatientReferenceNum
  ''::text                              as hicv_patientsecondinitial,           -- PatientSecondInitial
  (case when pypv_prov_provider_num != prov_provider_num then
        pypv_prov_provider_num
   else '' end)
                                        as hicv_payeeprovidernum,               -- PayeeProviderNum
  ''::text                              as hicv_paymentcategory,                -- PaymentCategory
  ''::text                              as hicv_periodtypeind,                  -- PeriodTypeInd
  ''::text                              as hicv_pharmacyprocessingcode,         -- PharmacyProcessingCode
  mdbt_batch_code                       as hicv_pmsclaimid,                     -- PmsClaimId
  mdbt_product                          as hicv_pmsproduct,                     -- PmsProduct
  mdbt_version                          as hicv_pmsversion,                     -- PmsVersion
  ''::text                              as hicv_prescriberid,                   -- PrescriberId
  ''::text                              as hicv_previoussupplies,               -- PreviousSupplies
  ''::text                              as hicv_principalprovidernum,           -- PrincipalProviderNum
  mdbt_recipient                        as hicv_recipient,                      -- Recipient
  to_char(patn_ref_date, 'DDMMYYYY')    as hicv_referraldatefrom,               -- ReferralDateFrom
  ''::text                              as hicv_referraldateto,                 -- ReferralDateTo
  ''::text                              as hicv_referralissuedate,              -- ReferralIssueDate
  ''::text                              as hicv_referraloverridetype,           -- ReferralOverrideType
  ''::text                              as hicv_referraloverridetypecde,        -- ReferralOverrideTypeCde
  patn_ref_period                       as hicv_referralperiod,                 -- ReferralPeriod
  ''::text                              as hicv_referralperiodtypecde,          -- ReferralPeriodTypeCde
  rfdr_provider                         as hicv_referringprovidernum,           -- ReferringProviderNum
  ''::text                              as hicv_reportstatuscde,                -- ReportStatusCde
  ''::text                              as hicv_requestcontenttype,             -- RequestContentType
  ''::text                              as hicv_requestissuedate,               -- RequestIssueDate
  ''::text                              as hicv_requesttypecde,                 -- RequestTypeCde
  ''::text                              as hicv_requesttypeind,                 -- RequestTypeInd
  ''::text                              as hicv_requestingprovidernum,          -- RequestingProviderNum
  ''::text                              as hicv_resubmissionind,                -- ResubmissionInd
  ''::text                              as hicv_rule3exemptind,                 -- Rule3ExemptInd
  ''::text                              as hicv_s4b3exemptind,                  -- S4B3ExemptInd
  ''::text                              as hicv_sddreasoncode,                  -- SDDReasonCode
  ''::text                              as hicv_selfdeemedcde,                  -- SelfDeemedCde
  ''::text                              as hicv_selfdeemedind,                  -- SelfDeemedInd
  ''::text                              as hicv_sender,                         -- Sender
  ''::text                              as hicv_sendercontactpersonname,        -- SenderContactPersonName
  ''::text                              as hicv_sendercontactpersonphone,       -- SenderContactPersonPhone
  ''::text                              as hicv_serialnum,                      -- SerialNum
  mdbt_server                           as hicv_server,                         -- Server
  svpf_desc                             as hicv_servicetext,                    -- ServiceText
  ''::text                              as hicv_servicetype,                    -- ServiceType
  'S'::text                             as hicv_servicetypecde,                 -- ServiceTypeCde
  prov_provider_num                     as hicv_servicingprovidernum,           -- ServicingProviderNum
  ''::text                              as hicv_sessionid,                      -- SessionId
  ''::text                              as hicv_timeduration,                   -- TimeDuration
  to_char(mdbt_claim_date, 'HH24MI')    as hicv_timeoflodgement,                -- TimeOfLodgement
  to_char(svpf_date_service,'HH24MI')   as hicv_timeofservice,                  -- TimeOfService
  ''::text                              as hicv_timeoftransmission,             -- TimeOfTransmission
  ''::text                              as hicv_transactionid,                  -- TransactionId
  ''::text                              as hicv_transmissionid,                 -- TransmissionId
  ''::text                              as hicv_transmissiontype,               -- TransmissionType
  ''::text                              as hicv_treatmentlocation,              -- TreatmentLocation
  ''::text                              as hicv_uniquepharmacyprescriptionnum,  -- UniquePharmacyPrescriptionNum
  ''::text                              as hicv_vaccinebatchnum,                -- VaccineBatchNum
  ''::text                              as hicv_vaccinecode,                    -- VaccineCode
  ''::text                              as hicv_vaccinedose,                    -- VaccineDose
  ''::text                              as hicv_veteranfilenum,                 -- VeteranFileNum

--
-- Location
--

  locn_address                          as hicv_locn_address_1,
  locn_address_2                        as hicv_locn_address_2,
  coalesce(locn_suburb,'') || '. ' || coalesce(locn_state,'') || ' ' || coalesce(locn_postcode, '')
                                        as hicv_locn_address_3,

--
-- associate some names with provider numbers
--

  pypv_prov_name                        as hicv_pypv_prov_name,                 -- name for PayeeProviderNum
  prov_name                             as hicv_prov_name,                      -- name for ServicingProviderNum
  rfdr_name                             as hicv_rfdr_name,                      -- name for ReferringProviderNum

-- 
-- mdbt sequence (session) and mdaf (voucher) ids
-- 

  mdbt__sequence                        as hicv_mdbt__sequence,                 -- DB concept of session/batch
  mdaf__sequence                        as hicv_mdaf__sequence,                 -- DB concept of voucher

--
-- Services
--

  
--
-- standard headers
--
 svpf__sequence                                 as hicv__sequence,              -- Row-ID
 svpf__timestamp                                as hicv__timestamp,             -- Timestamp
 svpf__user_entry                               as hicv__user_entry,            -- Entered by
 svpf__status                                   as hicv__status                 -- Record status
 --
 from ((((((patn LEFT JOIN mdaf ON (patn.patn__sequence = mdaf.mdaf_patn__sequence))
              LEFT JOIN mdbt ON (mdaf_mdbt__sequence = mdaf.mdaf_mdbt__sequence))
              LEFT JOIN svpf ON (svpf.svpf_mdaf__sequence = mdaf.mdaf__sequence))
              LEFT JOIN prov ON (prov.prov_code = mdbt.mdbt_prov_code))
              LEFT JOIN rfdr ON (rfdr.rfdr_code = mdaf.mdaf_rfdr_code))
              LEFT JOIN locn ON (locn.locn_id   = mdaf.mdaf_locn_id))
              LEFT JOIN pypv ON (pypv.pypv_prov_code = mdbt.mdbt_payee_prov_code)
;

grant select,insert,update on hicv to public;

-- Rules

-- Insert Rule
-- The insert is more flexibly handled by a function;
--    eg hicv_insert_row() in hicv-functions.sql

-- Update rule
-- drop rule rl_hicv_update on hicv; -- redundant as rule is dropped with view (above)
create rule rl_hicv_update as on update
       to hicv
       do instead
       (
        update  mdbt
        set     mdbt_batch_code = new.hicv_pmsclaimid,
                mdbt_claim_date = (substr(new.hicv_dateoflodgement, 1, 2) || '/' ||
                                   substr(new.hicv_dateoflodgement, 3, 2) || '/' ||
                                   substr(new.hicv_dateoflodgement, 5, 4) || ' ' ||
                                   substr(new.hicv_timeoflodgement, 1, 2) || ':' ||
                                   substr(new.hicv_timeoflodgement, 3, 2))::timestamp
        where   mdbt__sequence  = new.hicv_mdbt__sequence
       );


-- add the default metadata
select create_metadata('hicv');
update mtcl set mtcl_title = 'HIC Claims (View)' where mtcl_name = 'hicv';

-- update history;
\set mttb_name '\'hicv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

