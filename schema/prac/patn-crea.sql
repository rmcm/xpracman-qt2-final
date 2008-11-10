
--  -*- sql -*-
--  
--    $Id$

--  Copyright 1996 X=X Computer Software Trust
--   		  Kangaroo Ground Australia 3097


-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public Licensepublished by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
-- Report problems and direct all questions to:
 
--     Rex McMaster, rmcm@compsoft.com.au
 

 
-- Revision Log

create table patn                       -- Patient Demographics
(
 patn_flno text,                                 -- File number
 patn_psnam text,                                -- Surname
 patn_fsnam text,                                -- Firstname
 patn_title text,                                -- Title
 patn_dob timestamp,                             -- Date of birth
 patn_address text,                              -- Street
 patn_suburb text,                               -- Suburb
 patn_state text,                                -- State
 patn_postcode text,                             -- Postcode
 patn_phone text,                                -- Phone
 patn_phone_1 text,                              -- Extra phone
 patn_phone_2 text,                              -- Extra phone+
 patn_hlfd_code text,                            -- Health Fund
 patn_ins_level char,                            -- Insurance level
 patn_healthnumb text,                           -- Health Fund number
 patn_feet_code text default text '-' not null,  -- Fee level
 patn_medicare text,                             -- Medicare Number 
 patn_healthcard text,                           -- Healthcard number
 patn_dbtr_code text default text '-' not null,  -- Usual Debtor (code)
 patn_empl_code text default text '-' not null,  -- Usual Employer (code)
 patn_rfdr_code text default text '-' not null,  -- Usual Referrer (code)
 patn_ref_date timestamp,                        -- Referral date
 patn_ref_period integer,                        -- Referral period
 patn_claim_number text,                         -- Claim Number (WC)
 patn_accident_date timestamp,                   -- Date of accident (WC)
 patn_prov_code text default text '-' not null,  -- Usual   provider (code)
 patn_country text,                              -- Country of birth
 patn_aboriginality char,                        -- Aboriginality
 patn_sex char,                                  -- Gender
 patn_marital char,                              -- Marital Status
 patn_accl_code text,                            -- Account type
 patn_accommodation char,                        -- Accommodation type
 patn_care char,                                 -- Care type
 patn_patf_code text default text '-' not null,  -- patient flag
 patn_last_visit timestamp,                      -- last visit (syncd to evnv by trigger)
 patn_amount_outstanding numeric(12,2),          -- total amount outstanding- all debtors (syncd to invc trigger)
 patn_sync_debtor integer default 0 not null,    -- 0=disable 1=enable
 patn_fax_number text                            -- Fax Number
 patn_email_address text,                        -- Email Address
 --
 -- standard headers
 --
 patn__sequence serial not null,                             -- Row-ID
 patn__timestamp timestamp default now() not null,       -- Timestamp
 patn__user_entry text default CURRENT_USER not null,        -- Entered by
 patn__status char default text 'N' not null,                -- Record status
 --
 -- constraints
 --
 primary key (patn__sequence)

 );

create index ix_patn_dbtr on patn(patn_dbtr_code);
create index ix_patn_rfdr on patn(patn_rfdr_code);
create index ix_patn_psnam on patn(patn_psnam);
grant select,insert,update on patn to public;
grant select,insert,update on patn_patn__sequence_seq to public;

select create_metadata('patn');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('patn','$Id:');
