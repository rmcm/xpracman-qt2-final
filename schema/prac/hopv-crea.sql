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



-- Create VIew "hopv" based on "hopv"
--

-- \i schema/DBNAME/hopv-functions.sql   -- Functions need to be defined before the VIEW
--

drop view hopv;
create view hopv                         -- HIC-Online Patient Claims interface
  as select

--
-- HIC Elements
-- 

  to_char(patn_dob, 'DDMMYYYY')         as hopv_patientdateofbirth,             -- PatientDateOfBirth
  patn_psnam                            as hopv_patientfamilyname,              -- PatientFamilyName
  patn_fsnam                            as hopv_patientfirstname,               -- PatientFirstName
  substring(patn_medicare from '^[0-9]*')
                                        as hopv_patientmedicarecardnum,         -- PatientMedicareCardNum
  (case when length(patn_medicare) > 0 then
          substring(coalesce(substring(patn_medicare from '/[0-9]+'), '1') from '[0-9]+')
   else '' end)
                                        as hopv_patientreferencenum,            -- PatientReferenceNum
  ''::text                              as hopv_servicingprovidernum,           -- ServicingProviderNum
  ''::text                              as hopv_patientgender,                  -- PatientGender
  patn_hlfd_code                        as hopv_fundbrandid,                    -- FundBrandId
  patn_healthnumb                       as hopv_patientfundmembershipnum,       -- PatientFundMembershipNum
  ''::text                              as hopv_patientfundupi,                 -- PatientFundUPI
  ''::text                              as hopv_patientaliasfamilyname,         -- PatientAliasFamilyName
  ''::text                              as hopv_patientaliasfirstname,          -- PatientAliasFirstName



--
-- standard headers
--
 patn__sequence                                 as hopv__sequence,              -- Row-ID
 patn__timestamp                                as hopv__timestamp,             -- Timestamp
 patn__user_entry                               as hopv__user_entry,            -- Entered by
 patn__status                                   as hopv__status                 -- Record status
 --
 from patn
;

grant select,insert,update on hopv to public;

-- Rules

-- Insert Rule
-- The insert is more flexibly handled by a function;
--    eg hopv_insert_row() in hopv-functions.sql

-- Update rule
-- drop rule rl_hopv_update on hopv; -- redundant as rule is dropped with view (above)
-- create rule rl_hopv_update as on update
--        to hopv
--        do instead
--        (
--         update  invc
--         set     invc_claim_date  = new.hopv_timeoflodgement::timestamp
--         where   invc__sequence  = new.hopv__sequence
--        );
-- 

-- add the default metadata
select create_metadata('hopv');
update mtcl set mtcl_title = 'HIC Claims (View)' where mtcl_name = 'hopv';

-- update history;
\set mttb_name '\'hopv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

