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



-- Create VIew "himv" based on "hpcl"
--

-- \i schema/DBNAME/himv-functions.sql   -- Functions need to be defined before the VIEW
--
-- To create this from HICE
-- psql -q -A -t -d prac_43R4 -c "select hice_name from hice where hice_input = 'Y' order by hice_name" 
--     | awk '{printf("  ::text            as himv_%-50s-- %s\n",tolower($1)",",$1)}'

drop view himv;
create view himv                         -- HIC-Online Patient Claims interface
  as select

--
-- HIC Elements
-- 


-- Claim level attributes
        -- awk  '{q="\047\047";c=gensub(/([A-Z])/, "_\\1", "g");
                -- printf("  %-38.38s%-39.39s -- %s\n", "hicv"tolower(c), "as himv_"tolower($1)",", $1)}'

  (case when invc_balance(svpf_invc__sequence) = 0.0 then 'Y'
    else 'N' end)                       as himv_accountpaidind,                 -- AccountPaidInd
  ''::text                              as himv_accountreferenceid,             -- AccountReferenceId
  ''::text                              as himv_bankaccountname,                -- BankAccountName
  ''::text                              as himv_bankaccountnum,                 -- BankAccountNum
  ''::text                              as himv_bsbcode,                        -- BSBCode
  ''::text                              as himv_benefitassignmentauthorised,    -- BenefitAssignmentAuthorised
  ''::text                              as himv_billingagentid,                 -- BillingAgentId
  ''::text                              as himv_claimantaddressline1,           -- ClaimantAddressLine1
  ''::text                              as himv_claimantaddressline2,           -- ClaimantAddressLine2
  ''::text                              as himv_claimantaddresslocality,        -- ClaimantAddressLocality
  ''::text                              as himv_claimantaddresspostcode,        -- ClaimantAddressPostcode
  ''::text                              as himv_claimantdateofbirth,            -- ClaimantDateOfBirth
  ''::text                              as himv_claimantfamilyname,             -- ClaimantFamilyName
  ''::text                              as himv_claimantfirstname,              -- ClaimantFirstName
  ''::text                              as himv_claimantmedicarecardnum,        -- ClaimantMedicareCardNum
  ''::text                              as himv_claimantreferencenum,           -- ClaimantReferenceNum
  ''::text                              as himv_claimsubmissionauthorised,      -- ClaimSubmissionAuthorised
  ''::text                              as himv_claimtypecde,                   -- ClaimTypeCde
  ''::text                              as himv_facilityid,                     -- FacilityId
  ''::text                              as himv_fundbrandid,                    -- FundBrandId
  ''::text                              as himv_fundpayeeid,                    -- FundPayeeId
  ''::text                              as himv_principalprovidernum,           -- PrincipalProviderNum
  ''::text                              as himv_sendercontactpersonphone,       -- SenderContactPersonPhone
  ''::text                              as himv_sendercontactpersonname,        -- SenderContactPersonName


-- Voucher level attributes
        -- awk  '{q="\047\047";c=gensub(/([A-Z])/, "_\\1", "g");
        --      printf("  %-38.38s%-39.39s -- %s\n", q"::text", "as himv_"tolower($1)",", $1)}'

  ''::text                              as himv_admissiondate,                  -- AdmissionDate
  ''::text                              as himv_compensationclaimind,           -- CompensationClaimInd
  ''::text                              as himv_dischargedate,                  -- DischargeDate
  ''::text                              as himv_financialinterestdisclosureind, -- FinancialInterestDisclosureInd
  ''::text                              as himv_ifcissuecde,                    -- IFCIssueCde
  ''::text                              as himv_patientaliasfamilyname,         -- PatientAliasFamilyName
  ''::text                              as himv_patientaliasfirstname,          -- PatientAliasFirstName
  ''::text                              as himv_patientdateofbirth,             -- PatientDateOfBirth
  ''::text                              as himv_patientfamilyname,              -- PatientFamilyName
  ''::text                              as himv_patientfirstname,               -- PatientFirstName
  ''::text                              as himv_patientfundmembershipnum,       -- PatientFundMembershipNum
  ''::text                              as himv_patientfundupi,                 -- PatientFundUPI
  ''::text                              as himv_patientgender,                  -- PatientGender
  ''::text                              as himv_patientmedicarecardnum,         -- PatientMedicareCardNum
  ''::text                              as himv_patientreferencenum,            -- PatientReferenceNum
  ''::text                              as himv_patientsecondinitial,           -- PatientSecondInitial
  ''::text                              as himv_referralperiodtypecde,          -- ReferralPeriodTypeCde
  ''::text                              as himv_referralissuedate,              -- ReferralIssueDate
  ''::text                              as himv_referraloverridetypecde,        -- ReferralOverrideTypeCde
  ''::text                              as himv_referralperiod,                 -- ReferralPeriod
  ''::text                              as himv_referringprovidernum,           -- ReferringProviderNum
  ''::text                              as himv_requestissuedate,               -- RequestIssueDate
  ''::text                              as himv_requestingprovidernum,          -- RequestingProviderNum
  ''::text                              as himv_requesttypecde,                 -- RequestTypeCde
  ''::text                              as himv_servicetypecde,                 -- ServiceTypeCde
  ''::text                              as himv_servicingprovidernum,           -- ServicingProviderNum

-- Service level attributes 
        -- awk  '{q="\047\047";c=gensub(/([A-Z])/, "_\\1", "g");
                -- printf("  %-38.38s%-44.44s -- %s\n", q"::text", "as himv_"tolower($1)",", $1)}'

  ''::text                              as himv_aftercareoverrideind,           -- AfterCareOverrideInd
  ''::text                              as himv_chargeamount,                   -- ChargeAmount
  ''::text                              as himv_dateofservice,                  -- DateOfService
  ''::text                              as himv_duplicateserviceoverrideind,    -- DuplicateServiceOverrideInd
  ''::text                              as himv_equipmentid,                    -- EquipmentId
  ''::text                              as himv_fieldquantity,                  -- FieldQuantity
  ''::text                              as himv_itemnum,                        -- ItemNum
  ''::text                              as himv_lspnum,                         -- LSPNum
  ''::text                              as himv_multipleprocedureoverrideind,   -- MultipleProcedureOverrideInd
  ''::text                              as himv_noofpatientsseen,               -- NoOfPatientsSeen
  ''::text                              as himv_rule3exemptind,                 -- Rule3ExemptInd
  ''::text                              as himv_scpid,                          -- SCPId
  ''::text                              as himv_selfdeemedcde,                  -- SelfDeemedCde
  ''::text                              as himv_servicetext,                    -- ServiceText
  ''::text                              as himv_timeduration,                   -- TimeDuration
  ''::text                              as himv_timeofservice,                  -- TimeOfService

-- 
-- invc sequence (session) and hpcl (voucher) ids
-- 

  invc__sequence                        as himv_invc__sequence,                 -- DB concept of session/batch
  hpcl__sequence                        as himv_hpcl__sequence,                 -- DB concept of voucher

--
-- Services
--

  
--
-- standard headers
--
 svpf__sequence                                 as himv__sequence,              -- Row-ID
 svpf__timestamp                                as himv__timestamp,             -- Timestamp
 svpf__user_entry                               as himv__user_entry,            -- Entered by
 svpf__status                                   as himv__status                 -- Record status
 --
 from ((((((hpcl JOIN patn ON (patn.patn__sequence = hpcl.hpcl_patn__sequence))
              JOIN invc ON (invc.invc__sequence = hpcl.hpcl_invc__sequence))
              JOIN svpf ON (svpf.svpf_invc__sequence = hpcl.hpcl_invc__sequence))
              JOIN prov ON (prov.prov_code = invc.invc_prov_code))
              LEFT JOIN dbtr ON (dbtr.dbtr_code = invc.invc_dbtr_code))
              LEFT JOIN rfdr ON (rfdr.rfdr_code = hpcl.hpcl_rfdr_code))
              LEFT JOIN locn ON (locn.locn_id   = hpcl.hpcl_locn_code)
;

grant select,insert,update on himv to public;

-- Rules

-- Insert Rule
-- The insert is more flexibly handled by a function;
--    eg himv_insert_row() in himv-functions.sql

-- Update rule
-- drop rule rl_himv_update on himv; -- redundant as rule is dropped with view (above)
-- create rule rl_himv_update as on update
--        to himv
--        do instead
--        (
--         update  invc
--         set     invc_claim_date  = new.himv_timeoflodgement::timestamp
--         where   invc__sequence  = new.himv__sequence
--        );
-- 

-- add the default metadata
select create_metadata('himv');
update mtcl set mtcl_title = 'HIC Claims (View)' where mtcl_name = 'himv';

-- update history;
\set mttb_name '\'himv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

