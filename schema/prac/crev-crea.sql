--  -*- sql -*-
--  
--    $Id$

--  Copyright 2001 X=X Computer Software Trust
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
 

drop view crev;
create view crev                -- Credits (View)
 as select 
 paym_date_entry                        as crev_date_entry,
 paym_user_entry                        as crev_user_entry,
 paym_site_entry                        as crev_site_entry,
 paym_amount                            as crev_paym_amount,
 paym_tdtp_code                         as crev_tdtp_code,
 paym_drawer                            as crev_drawer,
 paym_bank                              as crev_bank,
 paym_branch                            as crev_branch,
 paym__sequence                         as crev_paym__sequence,
 cred_invc__sequence                    as crev_invc__sequence,
 cred_amount                            as crev_cred_amount,
 cred_gst_amount                        as crev_cred_gst_amount,
 cred_notes                             as crev_cred_notes,
 invc_balance(cred_invc__sequence)      as crev_invc_balance,

 --
 -- standard headers
 --
 cred__sequence                         as crev__sequence,              -- Row-ID
 cred__timestamp                        as crev__timestamp,             -- Timestamp
 cred__user_entry                       as crev__user_entry,            -- Entered by
 cred__status                           as crev__status                 -- Record status
 --
 from cred,paym
 where cred_paym__sequence = paym__sequence
;

grant select,insert,update,delete on crev to public;

-- and now - an update rule - should be here in crea file, as is dropped when view is dropped.
-- (creating view should be last procedure - ie after functions)
-- The insert is handled by crev_insert_row() in crev-functions.sql

create or replace rule rl_crev_update as on update
       to crev
       do instead
       (
         -- IMPORTANT!!!!
         -- update credit first (updates invc via trigger),
         --  then update paym_amount from aggregate
         update cred
         set    cred_invc__sequence  = new.crev_invc__sequence,
                cred_amount          = new.crev_cred_amount,
                cred_gst_amount      = new.crev_cred_gst_amount,
                cred_notes           = new.crev_cred_notes
         where  cred__sequence       = new.crev__sequence;
         update paym
         set    paym_user_entry      = new.crev_user_entry,
                paym_site_entry      = new.crev_site_entry,
                paym_date_entry      = new.crev_date_entry,
                paym_amount          = new.crev_paym_amount,
                paym_tdtp_code       = new.crev_tdtp_code,
                paym_drawer          = new.crev_drawer,
                paym_bank            = new.crev_bank,
                paym_branch          = new.crev_branch
         where  paym__sequence       = new.crev_paym__sequence;
         update paym
         set    paym_amount          = (select sum(cred_amount) + sum(cred_gst_amount)
                                        from   cred
                                        where  cred_paym__sequence  = new.crev_paym__sequence)
         where  paym__sequence       = new.crev_paym__sequence
       );

create or replace rule rl_crev_delete as on delete
       to crev
       do instead
       (
         delete from cred
         where  cred__sequence       = old.crev__sequence;
       );

select create_metadata('crev');

-- update history;
\set mttb_name '\'crev\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;


