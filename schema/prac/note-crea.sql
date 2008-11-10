--  -*- sql -*-
--
--     $Id$

--   Copyright 1997 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097


--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public Licensepublished by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
 
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
 
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
--  Report problems and direct all questions to:
 
--      Rex McMaster, rmcm@compsoft.com.au
 

 
--  Revision Log
  


create table note                       -- Patient Note pad -- 1. Patients -- 20
	(
         note_patn__sequence integer,   -- Patient ID--20--  --10--  --
         note_desc text,                -- Description--20--10--10--10--
         note__sequence serial primary key,        -- Row-ID -- -- 200 -- 200 --
         note__timestamp timestamp default now(),       -- Timestamp -- -- 200 -- 200 --
         note__user_entry text default CURRENT_USER,         -- Entered by -- -- 200 -- 200 --
         note__status char default text 'N'              -- Record status -- -- 200 -- 200 --
	);


grant select,insert,update on note to public;
grant select,insert,update on note_note__sequence_seq to public;


--Foreign--Patient--note_patn__sequence--patn--patn__sequence


select create_metadata('note');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('note','$Id:');
