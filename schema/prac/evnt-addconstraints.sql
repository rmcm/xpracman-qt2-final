-- -*- sql -*-
 
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
  
--
-- Foreign Key constraints
--

alter table evnt
        add constraint evnt_fk_patn
            foreign key (evnt_patn__sequence)
            references patn
            on update cascade
            on delete cascade
            initially deferred;

alter table evnt
        add constraint evnt_fk_prov
            foreign key (evnt_prov_code)
            references prov
            on update cascade
            on delete restrict
            initially deferred;

alter table evnt
        add constraint evnt_fk_rfdr
            foreign key (evnt_rfdr_code)
            references rfdr
            on update cascade
            on delete restrict
            initially deferred;

alter table evnt
        add constraint evnt_fk_locn
            foreign key (evnt_locn_code)
            references locn
            on update cascade
            on delete restrict
            initially deferred;

alter table evnt
        add constraint evnt_fk_aptp
            foreign key (evnt_aptp_code)
            references aptp
            on update cascade
            on delete restrict
            initially deferred;

alter table evnt
        add constraint evnt_fk_apst
            foreign key (evnt_apst_code)
            references apst
            on update cascade
            on delete restrict
            initially deferred;

-- update history;
insert into mttb(mttb_name, mttb_rcs_header, mttb_notes) values('evnt','$Id$', 'Triggers');

