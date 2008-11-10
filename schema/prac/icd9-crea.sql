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
  
create table icd9                       -- ICD-9 Codes -- 6. Episodes -- 1000
	(
	icd9_codetype char,             -- Code type-- -- 30 -- 30 -- 30 --
	icd9_code text,                 -- ICD9 code-- -- 30 -- 30 -- 30 --
	icd9_age_edit char,             -- Age limit-- -- 30 -- 30 -- 30 --
	icd9_low_age smallint,          -- Low age-- -- 30 -- 30 -- 30 --
	icd9_high_age smallint,         -- High Age-- -- 30 -- 30 -- 30 --
	icd9_sex_edit char,             -- Gender limit-- -- 30 -- 30 -- 30 --
	icd9_code_edit char,            -- Code edit-- -- 30 -- 30 -- 30 --
	icd9_area_edit char,            -- Area Edit-- -- 30 -- 30 -- 30 --
	icd9_digit_5 text,              -- Fifth digit edit-- -- 30 -- 30 -- 30 --
	icd9_desc text,                 -- Description-- -- 30 -- 30 -- 30 --
        icd9__sequence serial primary key,         -- Row-ID -- -- 200 -- 200 --
        icd9__timestamp timestamp default now(),        -- Timestamp -- -- 200 -- 200 --
        icd9__user_entry text default CURRENT_USER,          -- Entered by -- -- 200 -- 200 --
        icd9__status char default text 'N'               -- Record status -- -- 200 -- 200 --
	);



grant select,insert,update on icd9 to public;
grant select,insert,update on icd9_icd9__sequence_seq to public;


select create_metadata('icd9');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('icd9','$Id:');
