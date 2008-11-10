--  -*- sql -*-
--
 
--    $Id$

--  Copyright 1999 X=X Computer Software Trust
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

create table clst                               -- Service Clusters -- 3. Billing -- 100
	(
        clst_serv_code_parent text,             -- Cluster Code-- -- 5 -- 5 -- 5 --
	clst_serv_code_child text,              -- Service code-- -- 10 -- 10 -- 10 --
	clst_feet_code text,                    -- Fee level-- -- 20 -- 20 -- 20 --
        clst__sequence serial primary key,                 -- Row-ID -- -- 200 -- 200 --
        clst__timestamp timestamp default now(),                -- Timestamp -- -- 200 -- 200 --
        clst__user_entry text default CURRENT_USER,                  -- Entered by -- -- 200 -- 200 --
        clst__status char default text 'N'                       -- Record status -- -- 200 -- 200 --
	);


create index ix_clst_1 on clst(clst_serv_code_parent);

grant select,insert,update on clst to public;
grant select,insert,update on clst_clst__sequence_seq to public;


--Foreign--Cluster Parent--clst_serv_code_parent--serv--serv_code
--Foreign--Component--clst_serv_code_child--serv--serv_code
--Foreign--Fee Type--_feet_code--feet--feet_code


select create_metadata('clst');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('clst','$Id:');
