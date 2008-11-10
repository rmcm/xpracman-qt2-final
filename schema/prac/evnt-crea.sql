--  -*- sql -*-
--
--     $Id$

--   Copyright 2001 X=X Computer Software Trust
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
  


create table evnt                                               -- Patient events - schedules, appt etc
(
 evnt_patn__sequence integer default 0 not null,                -- Patient id
 evnt_prov_code text default text '-' not null,                 -- Provider code
 evnt_rfdr_code text default text '-' not null,                 -- Referror code
 evnt_locn_code text default text '-' not null,                 -- Location code
 evnt_aptp_code text default text '-' not null,                 -- Appointment type
 evnt_starttime timestamp,                                      -- start time - start as null
 evnt_duration interval default '15 minutes' not null,          -- duration
 evnt_desc text,                                                -- Description
 evnt_note_1 text,                                              -- user note - 1
 evnt_note_2 text,                                              -- user note - 2
 evnt_note_3 text,                                              -- user note - 3
 evnt_note_4 text,                                              -- user note - 4
 evnt_note_5 text,                                              -- user note - 5
 evnt_note_6 text,                                              -- user note - 6
 evnt_note_7 text,                                              -- user note - 7
 evnt_note_8 text,                                              -- user note - 8
 evnt_note_9 text,                                              -- user note - 9
 evnt_apst_code text default text '-' not null,                 -- Appointment Status
 --
 -- standard headers
 --
 evnt__sequence serial not null,                                -- Row-ID
 evnt__timestamp timestamp default now() not null,         -- Timestamp
 evnt__user_entry text default CURRENT_USER not null,           -- Entered by
 evnt__status char default text 'N' not null,                   -- Record status
 --
 -- constraints
 --
 primary key (evnt__sequence)

);

create index ix_evnt_starttime on evnt(evnt_starttime);

grant select,insert,update on evnt to public;
grant select,insert,update on evnt_evnt__sequence_seq to public;

select create_metadata('evnt');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('evnt','$Id$');

