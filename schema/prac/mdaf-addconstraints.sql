--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
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


alter table mdaf
      add constraint mdaf_fk_patn
          foreign key(mdaf_patn__sequence)
          references patn
          on update cascade
          on delete restrict
          initially deferred;

alter table mdaf
      add constraint mdaf_fk_prov
          foreign key(mdaf_prov_code)
          references prov
          on update cascade
          on delete restrict
          initially deferred;

alter table mdaf
      add constraint mdaf_fk_rfdr
          foreign key(mdaf_rfdr_code)
          references rfdr
          on update cascade
          on delete restrict
          initially deferred;

alter table mdaf
      add constraint mdaf_fk_mdbt
          foreign key(mdaf_mdbt__sequence)
          references mdbt
          on update cascade
          on delete restrict
          initially deferred;


-- update history;
\set mttb_name '\'mdaf\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
