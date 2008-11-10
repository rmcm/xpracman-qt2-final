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

create table lthd                       -- Letterheads
(
 lthd_prov_code text,                    -- Letterhead Code 
 lthd_target_id text,                    -- target object eg INVOICE or BANKLIST
 lthd_order integer,                     -- Display Order
 lthd_type text,                         -- Type
 lthd_data text,                         -- Content
 lthd_x_coord integer,                   -- X Co-ordinate
 lthd_y_coord integer,                   -- Y Co-ordinate
 lthd_scale numeric(3,1),                -- Scale
 lthd_font text,                         -- Font Name
 lthd_size numeric(4,1),                 -- Font Size (points)
 lthd_align char(1),                     -- Alignment (L/R/C)
 --
 -- standard headers
 --
 lthd__sequence serial not null,                              -- Row-ID
 lthd__timestamp timestamp default now() not null,        -- Timestamp
 lthd__user_entry text default CURRENT_USER not null,         -- Entered by
 lthd__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(lthd__sequence)
);

grant select,insert,update on lthd to public;
grant select,insert,update on lthd_lthd__sequence_seq to public;


select create_metadata('lthd');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('lthd','$Id:');
