--     $Id$
-- 
--   Copyright 2004 X=X Computer Software Trust
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




-- Create a view of the pate with MD2 export structure
-- Create VIew "pemd" based on "patn"
--

-- \i schema/DBNAME/pemd-functions.sql   -- Functions need to be defined before the VIEW

drop view pemd;
create view pemd                         -- MD2 export structure
  as select
  
 rpad(patn_flno,9) ||                                   -- 1
 rpad(coalesce(patn_title, ''),5) ||                    -- 2
 rpad(coalesce(patn_psnam, ''),30) ||                   -- 3
 rpad(coalesce(patn_fsnam, ''),30) ||                   -- 4
 rpad(coalesce(patn_address, ''),40) ||                 -- 5
 rpad(coalesce(patn_suburb, ''),25) ||                  -- 6
 rpad(coalesce(patn_postcode, ''),4) ||                 -- 7
 rpad(coalesce(date(patn_dob)::text, ''),10) ||         -- 8
 rpad(coalesce(patn_medicare, ''),12) ||                -- 9
 rpad(coalesce(patn_medicare, ''),1) ||                 -- 10
 rpad(coalesce(patn_healthcard, ''),14) ||              -- 11
 rpad(coalesce(patn_healthcard, ''),14) ||              -- 12
 rpad(coalesce(patn_phone, ''),14) ||                   -- 13
 rpad(coalesce(patn_phone, ''),14) ||                   -- 14
 rpad(' ',1) ||                                         -- 15
 rpad(coalesce(patn_sex, ''),1) ||                      -- 16
 rpad(' ',14) ||                                        -- 17
 rpad(coalesce(patn_flno, ''),10) ||                    -- 18
 rpad(' ',9) ||                                         -- 19
 rpad(' ',1) ||                                         -- 20
 rpad(' ',100) ||                                       -- 21
 rpad(coalesce(rfdr_name, ''),50) ||                    -- 22
 rpad(coalesce(date(patn_ref_date)::text,'' ),10) ||    -- 23
 rpad((case when patn_ref_period is null                
                  or patn_ref_period = '99' then        
           '00' else '12' end),2) ||                    -- 24
 rpad(coalesce(patn_healthnumb, ''),20) ||              -- 25
 rpad(coalesce(hlfd_desc, ''),40) ||                    -- 26
 rpad('',10) ||                                         -- 27
 rpad('N',1) ||                                         -- 28
 rpad(' ',14)            as pemd_export,                -- 29

--
-- standard headers
--
 pate_patn__sequence                    as pemd__sequence,              -- Row-ID
 pate__timestamp                        as pemd__timestamp,             -- Timestamp
 pate__user_entry                       as pemd__user_entry,            -- Entered by
 pate__status                           as pemd__status                 -- Record status
 --
 from  ((pate JOIN patn on (patn.patn__sequence = pate.pate_patn__sequence))
         LEFT JOIN hlfd on (hlfd.hlfd_code = patn.patn_hlfd_code))
         LEFT JOIN rfdr on (rfdr.rfdr_code = patn.patn_rfdr_code)
;

grant select on pemd to public;
select create_metadata('pemd');
update mtcl set mtcl_title = 'Pending Exports - MD2' where mtcl_name = 'pemd';
update mtat set mtat_input_method = 'RO' where mtat_class_name = 'pemd' and mtat_input_method != 'SYSTEM';

-- update history;
\set mttb_name '\'pemd\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
