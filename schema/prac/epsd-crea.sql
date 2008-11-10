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
  
create table epsd                       -- Treatment Episodes -- 6. Episodes -- 1000
	(
	epsd_admit_date timestamp,       -- Admission date-- -- 30 -- 30 -- 30 --
	epsd_admit_criteria char,       -- Admission criteria -- -- 30 -- 30 -- 30 --
	epsd_admit_type text,           -- Admission type-- -- 30 -- 30 -- 30 --
	epsd_admit_source text,         -- Admission source-- -- 30 -- 30 -- 30 --
	epsd_neonate_weight smallint,   -- Neonate weight-- -- 30 -- 30 -- 30 --
	epsd_trans_source text,         -- Transfer source-- -- 30 -- 30 -- 30 --
	epsd_healthfund text,           -- Healthfund-- -- 30 -- 30 -- 30 --
	epsd_ins_level text,            -- Insurance level-- -- 30 -- 30 -- 30 --
	epsd_speciality text,           -- Clinical speciality-- -- 30 -- 30 -- 30 --
	epsd_lvday_month smallint,      -- Leave days MTD-- -- 30 -- 30 -- 30 --
	epsd_lvday_finyr smallint,      -- Leave days YTD-- -- 30 -- 30 -- 30 --
	epsd_lvday_total smallint,      -- Leave days total-- -- 30 -- 30 -- 30 --
	epsd_sepn_date timestamp,         -- Separation date-- -- 30 -- 30 -- 30 --
	epsd_sepn_type text,            -- Separation type-- -- 30 -- 30 -- 30 --
	epsd_sepn_transfer text,        -- Separation transfer-- -- 30 -- 30 -- 30 --
	epsd_re_admit text,             -- Intention to readmit-- -- 30 -- 30 -- 30 --
	epsd_user_flag_sepn text,       -- Separation flag-- -- 30 -- 30 -- 30 --
	epsd_study text,                -- Study ID-- -- 30 -- 30 -- 30 --
	epsd_user_flag_diag text,       -- Diagnosis flag-- -- 30 -- 30 -- 30 --
        epsd_patn__sequence integer,    -- Patient OID-- -- 30 -- 30 -- 30 --
        epsd__sequence serial primary key,         -- Row-ID -- -- 200 -- 200 --
        epsd__timestamp timestamp default now(),        -- Timestamp -- -- 200 -- 200 --
        epsd__user_entry text default CURRENT_USER,          -- Entered by -- -- 200 -- 200 --
        epsd__status char default text 'N'               -- Record status -- -- 200 -- 200 --
	);



grant select,insert,update on epsd to public;
grant select,insert,update on epsd_epsd__sequence_seq to public;


select create_metadata('epsd');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('epsd','$Id:');
