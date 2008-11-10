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



-- Create VIew "hpcv" based on "hpcl"
--

-- \i schema/DBNAME/hpcv-functions.sql   -- Functions need to be defined before the VIEW
--
-- To create this from HICE
-- psql -q -A -t -d prac_43R4 -c "select hice_name from hice where hice_input = 'Y' order by hice_name" 
--     | awk '{printf("  ::text            as hpcv_%-50s-- %s\n",tolower($1)",",$1)}'

drop view hpcv;
create view hpcv                         -- HIC-Online Patient Claims interface
  as select

--
-- HIC Elements
-- 


-- Claim level attributes
        -- awk  '{q="\047\047";c=gensub(/([A-Z])/, "_\\1", "g");
                -- printf("  %-38.38s%-39.39s -- %s\n", "hicv"tolower(c), "as hpcv_"tolower($1)",", $1)}'

  (case when invc_balance(svpf_invc__sequence) = 0.0 then 'Y'
    else 'N' end)                       as hpcv_accountpaidind,                 -- AccountPaidInd
  ''::text                              as hpcv_claimsubmissionauthorised,      -- ClaimSubmissionAuthorised
  hpcl_dateoflodgement                  as hpcv_dateoflodgement,                -- DateOfLodgement
  to_char(patn_dob, 'DDMMYYYY')         as hpcv_patientdateofbirth,             -- PatientDateOfBirth
  patn_psnam                            as hpcv_patientfamilyname,              -- PatientFamilyName
  patn_fsnam                            as hpcv_patientfirstname,               -- PatientFirstName
  substring(patn_medicare from '^[0-9]*')
                                        as hpcv_patientmedicarecardnum,         -- PatientMedicareCardNum
  (case when length(patn_medicare) > 0 then
          substring(coalesce(substring(patn_medicare from '/[0-9]+'), '1') from '[0-9]+')
   else '' end)
                                        as hpcv_patientreferencenum,            -- PatientReferenceNum
  prov_provider_num                     as hpcv_servicingprovidernum,           -- ServicingProviderNum
  prov_name                             as hpcv_servicing_prov_name,            -- provider name
  hpcl_timeoflodgement                  as hpcv_timeoflodgement,                -- TimeOfLodgement
  ''::text                              as hpcv_accountreferenceid,             -- AccountReferenceId
  ''::text                              as hpcv_bankaccountname,                -- BankAccountName
  ''::text                              as hpcv_bankaccountnum,                 -- BankAccountNum
  ''::text                              as hpcv_bsbcode,                        -- BSBCode
  ''::text                              as hpcv_claimantaddressline1,           -- ClaimantAddressLine1
  ''::text                              as hpcv_claimantaddressline2,           -- ClaimantAddressLine2
  ''::text                              as hpcv_claimantaddresslocality,        -- ClaimantAddressLocality
  ''::text                              as hpcv_claimantaddresspostcode,        -- ClaimantAddressPostcode
  ''::text                              as hpcv_claimantdateofbirth,            -- ClaimantDateOfBirth
  ''::text                              as hpcv_claimantfamilyname,             -- ClaimantFamilyName
  ''::text                              as hpcv_claimantfirstname,              -- ClaimantFirstName
  ''::text                              as hpcv_claimantmedicarecardnum,        -- ClaimantMedicareCardNum
  ''::text                              as hpcv_claimantreferencenum,           -- ClaimantReferenceNum
  patn_phone                            as hpcv_contactphonenum,                -- ContactPhoneNum
  ''::text                              as hpcv_payeeprovidernum,               -- PayeeProviderNum
  ''::text                              as hpcv_transactionid,                  -- TransactionId

  hpcl_storeandforward                  as hpcv_storeandforward,                -- Store and Forward claim

-- Voucher level attributes
        -- awk  '{q="\047\047";c=gensub(/([A-Z])/, "_\\1", "g");
        --      printf("  %-38.38s%-39.39s -- %s\n", q"::text", "as hpcv_"tolower($1)",", $1)}'

  to_char(svpf_date_service,'DDMMYYYY') as hpcv_dateofservice,                  -- DateOfService
  ''::text                              as hpcv_timeofservice,                  -- TimeOfService
  ''::text                              as hpcv_referraloverridetypecde,        -- ReferralOverrideTypeCde
  (case when length(invc_rfdr_code) > 0 and invc_rfdr_code != '-' then
        rfdr_provider else '' end)
                                        as hpcv_referringprovidernum,           -- ReferringProviderNum
  ''::text                              as hpcv_requestingprovidernum,          -- RequestingProviderNum
  ''::text                              as hpcv_referralissuedate,              -- ReferralIssueDate
  ''::text                              as hpcv_requestissuedate,               -- RequestIssueDate
 (case when length(invc_rfdr_code) > 0 and invc_rfdr_code != '-' then
    case when invc_rfdr_period  between 1 and 12 then 'S' 
        else case when invc_rfdr_period = 99 then 'I' 
        else 'N' end end 
  else '' end)
                                        as hpcv_referralperiodtypecde,          -- ReferralPeriodTypeCde
  ''::text                              as hpcv_requesttypecde,                 -- RequestTypeCde

  ''::text                              as hpcv_aftercareoverrideind,           -- AfterCareOverrideInd
  (svpf_amount*100)::integer            as hpcv_chargeamount,                   -- ChargeAmount
  svpf_amount                           as hpcv_svpf_amount,
  svpf_desc                             as hpcv_svpf_desc,
  ''::text                              as hpcv_duplicateserviceoverrideind,    -- DuplicateServiceOverrideInd
  ''::text                              as hpcv_equipmentid,                    -- EquipmentId
  ''::text                              as hpcv_fieldquantity,                  -- FieldQuantity
  ''::text                              as hpcv_hospitalind,                    -- HospitalInd
  svpf_serv_code                        as hpcv_itemnum,                        -- ItemNum
  ''::text                              as hpcv_lccid,                          -- LCCId
  ''::text                              as hpcv_lspnum,                         -- LSPNum
  ''::text                              as hpcv_multipleprocedureoverrideind,   -- MultipleProcedureOverrideInd
  ''::text                              as hpcv_noofpatientsseen,               -- NoOfPatientsSeen
  fn_guess_credit(svpf__sequence)       as hpcv_patientcontribamt,              -- PatientContribAmt
  ''::text                              as hpcv_rule3exemptind,                 -- Rule3ExemptInd
  ''::text                              as hpcv_s4b3exemptind,                  -- S4B3ExemptInd
  ''::text                              as hpcv_selfdeemedcde,                  -- SelfDeemedCde
  ''::text                              as hpcv_servicetext,                    -- ServiceText
  ''::text                              as hpcv_timeduration,                   -- TimeDuration

-- 
-- invc sequence (session) and hpcl (voucher) ids
-- 

  invc__sequence                        as hpcv_invc__sequence,                 -- DB concept of session/batch
  hpcl__sequence                        as hpcv_hpcl__sequence,                 -- DB concept of voucher

--
-- Claim results
--

  svmc_rsn_code                         as hpcv_rsn_code,                       -- Result code
  svmc_benefit_amount                   as hpcv_benefit_amount,                 -- Benefit amount

--
-- Services
--

  
--
-- standard headers
--
 svpf__sequence                                 as hpcv__sequence,              -- Row-ID
 svpf__timestamp                                as hpcv__timestamp,             -- Timestamp
 svpf__user_entry                               as hpcv__user_entry,            -- Entered by
 svpf__status                                   as hpcv__status                 -- Record status
 --
 from (((((((hpcl JOIN patn ON (patn.patn__sequence = hpcl.hpcl_patn__sequence))
              JOIN invc ON (invc.invc__sequence = hpcl.hpcl_invc__sequence))
              JOIN svpf ON (svpf.svpf_invc__sequence = hpcl.hpcl_invc__sequence))
              JOIN prov ON (prov.prov_code = invc.invc_prov_code))
              LEFT JOIN dbtr ON (dbtr.dbtr_code = invc.invc_dbtr_code))
              LEFT JOIN rfdr ON (rfdr.rfdr_code = hpcl.hpcl_rfdr_code))
              LEFT JOIN locn ON (locn.locn_id   = hpcl.hpcl_locn_code))
              LEFT JOIN svmc ON (svmc.svmc_svpf__sequence = svpf.svpf__sequence)
;

grant select,insert,update on hpcv to public;

-- Rules

-- Insert Rule
-- The insert is more flexibly handled by a function;
--    eg hpcv_insert_row() in hpcv-functions.sql

-- Update rule
 drop rule rl_hpcv_update on hpcv; -- redundant as rule is dropped with view (above)
 create rule rl_hpcv_update as on update
        to hpcv
        do instead
        (
        -- insert svmc record if not exists
        insert  into svmc(svmc_svpf__sequence, svmc_rsn_code, svmc_benefit_amount)
        (select new.hpcv__sequence, new.hpcv_rsn_code, new.hpcv_benefit_amount
         where  not exists(select  svmc_svpf__sequence, svmc_rsn_code, svmc_benefit_amount
                           from    svmc
                           where   svmc_svpf__sequence = new.hpcv__sequence));
        -- update svmc with the rsn and benefit amount
        update svmc
         set    svmc_rsn_code = new.hpcv_rsn_code,
                svmc_benefit_amount = new.hpcv_benefit_amount
         where  svmc_svpf__sequence  = new.hpcv__sequence;
        );
 

-- add the default metadata
select create_metadata('hpcv');
update mtcl set mtcl_title = 'HIC Claims (View)' where mtcl_name = 'hpcv';

-- update history;
\set mttb_name '\'hpcv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

