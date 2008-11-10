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

drop view pmsc;
create view pmsc                -- Payment Summary - credits
 as select 

 cred_invc__sequence                    as pmsc_invc__sequence,
 cred_paym__sequence                    as pmsc_paym__sequence,
 cred_amount+cred_gst_amount            as pmsc_total_amount,
 cred_amount                            as pmsc_amount,
 cred_gst_amount                        as pmsc_gst_amount,
 to_char(cred__timestamp,'YYYY-MM')     as pmsc_month,
 --
 -- standard headers
 --
 cred__sequence                         as pmsc__sequence,      -- Row-ID
 now()                                  as pmsc__timestamp,     -- Timestamp
 CURRENT_USER                           as pmsc__user_entry,    -- Entered by
 text 'N'                               as pmsc__status         -- Record status
 --
 from cred
-- where cred_amount+cred_gst_amount!=0.00::numeric(12,2)
;

grant select on pmsc to public;

select create_metadata('pmsc');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('pmsc','$Id:');
