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



create table hima                       -- HIC Patient IMC Claim Archive
(

 -- Claim Header:

  hima_accountpaidind text,                                     -- AccountPaidInd
  hima_accountreferenceid text,                                 -- AccountReferenceId
  hima_bankaccountname text,                                    -- BankAccountName
  hima_bankaccountnum text,                                     -- BankAccountNum
  hima_bsbcode text,                                            -- BSBCode
  hima_benefitassignmentauthorised text,                        -- BenefitAssignmentAuthorised
  hima_billingagentid text,                                     -- BillingAgentId
  hima_claimantaddressline1 text,                               -- ClaimantAddressLine1
  hima_claimantaddressline2 text,                               -- ClaimantAddressLine2
  hima_claimantaddresslocality text,                            -- ClaimantAddressLocality
  hima_claimantaddresspostcode text,                            -- ClaimantAddressPostcode
  hima_claimantdateofbirth text,                                -- ClaimantDateOfBirth
  hima_claimantfamilyname text,                                 -- ClaimantFamilyName
  hima_claimantfirstname text,                                  -- ClaimantFirstName
  hima_claimantmedicarecardnum text,                            -- ClaimantMedicareCardNum
  hima_claimantreferencenum text,                               -- ClaimantReferenceNum
  hima_claimsubmissionauthorised text,                          -- ClaimSubmissionAuthorised
  hima_claimtypecde text,                                       -- ClaimTypeCde
  hima_facilityid text,                                         -- FacilityId
  hima_fundbrandid text,                                        -- FundBrandId
  hima_fundpayeeid text,                                        -- FundPayeeId
  hima_principalprovidernum text,                               -- PrincipalProviderNum
  hima_sendercontactpersonphone text,                           -- SenderContactPersonPhone
  hima_sendercontactpersonname text,                            -- SenderContactPersonName

 -- Voucher Elements:

  hima_admissiondate text,                                      -- AdmissionDate
  hima_compensationclaimind text,                               -- CompensationClaimInd
  hima_dischargedate text,                                      -- DischargeDate
  hima_financialinterestdisclosureind text,                     -- FinancialInterestDisclosureInd
  hima_ifcissuecde text,                                        -- IFCIssueCde
  hima_patientaliasfamilyname text,                             -- PatientAliasFamilyName
  hima_patientaliasfirstname text,                              -- PatientAliasFirstName
  hima_patientdateofbirth text,                                 -- PatientDateOfBirth
  hima_patientfamilyname text,                                  -- PatientFamilyName
  hima_patientfirstname text,                                   -- PatientFirstName
  hima_patientfundmembershipnum text,                           -- PatientFundMembershipNum
  hima_patientfundupi text,                                     -- PatientFundUPI
  hima_patientgender text,                                      -- PatientGender
  hima_patientmedicarecardnum text,                             -- PatientMedicareCardNum
  hima_patientreferencenum text,                                -- PatientReferenceNum
  hima_patientsecondinitial text,                               -- PatientSecondInitial
  hima_referralperiodtypecde text,                              -- ReferralPeriodTypeCde
  hima_referralissuedate text,                                  -- ReferralIssueDate
  hima_referraloverridetypecde text,                            -- ReferralOverrideTypeCde
  hima_referralperiod text,                                     -- ReferralPeriod
  hima_referringprovidernum text,                               -- ReferringProviderNum
  hima_requestissuedate text,                                   -- RequestIssueDate
  hima_requestingprovidernum text,                              -- RequestingProviderNum
  hima_requesttypecde text,                                     -- RequestTypeCde
  hima_servicetypecde text,                                     -- ServiceTypeCde
  hima_servicingprovidernum text,                               -- ServicingProviderNum

 -- Service Elements:

  hima_aftercareoverrideind text,                               -- AfterCareOverrideInd
  hima_chargeamount text,                                       -- ChargeAmount
  hima_dateofservice text,                                      -- DateOfService
  hima_duplicateserviceoverrideind text,                        -- DuplicateServiceOverrideInd
  hima_equipmentid text,                                        -- EquipmentId
  hima_fieldquantity text,                                      -- FieldQuantity
  hima_itemnum text,                                            -- ItemNum
  hima_lspnum text,                                             -- LSPNum
  hima_multipleprocedureoverrideind text,                       -- MultipleProcedureOverrideInd
  hima_noofpatientsseen text,                                   -- NoOfPatientsSeen
  hima_rule3exemptind text,                                     -- Rule3ExemptInd
  hima_scpid text,                                              -- SCPId
  hima_selfdeemedcde text,                                      -- SelfDeemedCde
  hima_servicetext text,                                        -- ServiceText
  hima_timeduration text,                                       -- TimeDuration
  hima_timeofservice text,                                      -- TimeOfService

  hima_transactionid text,                                      --  transactionid
 
 --
 -- standard headers
 --
 hima__sequence serial not null,                              -- Row-ID
 hima__timestamp timestamp default now() not null,       -- Timestamp
 hima__user_entry text default CURRENT_USER not null,         -- Entered by
 hima__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(hima__sequence)
 );

grant select,insert,update on hima to public;
grant select,insert,update on hima_hima__sequence_seq to public;

select create_metadata('hima');
update mtcl set mtcl_title = 'HIC Patient IMC Archive' where mtcl_name = 'hima';

update mtat set mtat_input_method = 'RO' where mtat_class_name = 'hima';

update mtat set mtat_title = 'Transaction ID', mtat_displen = 15 where mtat_name = 'hima_transactionid';

-- update history;
\set mttb_name '\'hima\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

