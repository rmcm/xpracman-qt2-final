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

drop view fept;
create view fept                -- Aged debt view
 as select 

 feeb_feet_code                         as fept_feet_code,
 feeb_serv_code                         as fept_serv_code,
 serv_gst_percentage                    as fept_gst_percentage,
 feeb_amount                            as fept_amount,
 to_char((feeb_amount*serv_gst_percentage)/100::numeric(12,2), '00.00')::numeric
                                        as fept_gst_amount,
 serv_desc                              as fept_desc,
 --
 -- standard headers
 --
 feeb__sequence                         as fept__sequence,      -- Row-ID
 now()                                  as fept__timestamp,     -- Timestamp
 CURRENT_USER                           as fept__user_entry,    -- Entered by
 text 'N'                               as fept__status         -- Record status
 --
 from   feeb,serv
 where  serv_code = feeb_serv_code
;

grant select on fept to public;

select create_metadata('fept');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('fept','$Id:');
