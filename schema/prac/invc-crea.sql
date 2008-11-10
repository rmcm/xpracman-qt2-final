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

create table invc                       -- Invoices
(
 --
 -- Single debtor, creditor provider and patient per invoice
 --
 invc_dbtr_code text default text '-' not null,      -- Debtor ID code
 invc_bank_code text default text '-' not null,      -- Creditor code
 invc_prov_code text default text '-' not null,      -- Provider Code
 invc_patn__sequence integer default 0 not null,     -- Patient
 --
 -- The following are copied from the patn record at the time of invoice creation
 -- and represent "snapshot" at that time.
 --
 invc_empl_code text default text '-' not null,      -- Employer (used for messages on invoice)
 invc_feet_code text default text '-' not null,      -- Fee level (used for messages on invoice)
 invc_rfdr_code text default text '-' not null,      -- Referrer code (used for messages on invoice)
 invc_rfdr_date timestamp,                           -- Referral date (used for messages on invoice)
 invc_rfdr_period text,                              -- Referral period (used for messages on invoice)
 invc_hlfd_code text,
 invc_ins_level text,
 invc_healthnumb text,
 invc_healthcard text,
 invc_medicare text
 invc_claim_number text,
 invc_accident_date timestamp,
 invc_reference_1 text,
 invc_reference_2 text,
 invc_reference_3 text,
 

 --
 -- timestamps
 --
 invc_date_created timestamp default now(),      -- Date created
 invc_date_printed timestamp,                         -- Date printed
 invc_date_reprint timestamp,                         -- Date re-printed
 --
 -- Totals - updated by triggers on svpf and cred
 --
 invc_amount numeric(12,2) default numeric(12,2) '0.00' not null,             -- Amount-- == sum(svpf_amount)
 invc_paid_amount numeric(12,2) default numeric(12,2) '0.00' not null,        -- Paid to invoice (ex GST) == sum(cred_amount)
 invc_gst_amount numeric(12,2) default numeric(12,2) '0.00' not null,         -- GST == sum(svpf_gst_amount) 
 invc_paid_gst_amount numeric(12,2) default numeric(12,2) '0.00' not null,    -- paid to invoice (GST) = sum(cred_gst_amount)
 --
 -- standard headers
 --
 invc__sequence serial not null,                              -- Row-ID
 invc__timestamp timestamp default now() not null,        -- Timestamp
 invc__user_entry text default CURRENT_USER not null,         -- Entered by
 invc__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(invc__sequence)
 );

create index ix_invc_dbtr on invc(invc_dbtr_code);
create index ix_invc_patn on invc(invc_patn__sequence);

grant select,insert,update on invc to public;
grant select,insert,update on invc_invc__sequence_seq to public; 

select create_metadata('invc');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('invc','$Id:');
