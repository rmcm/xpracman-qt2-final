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
  
alter table invc
      add constraint invc_fk_dbtr
          foreign key (invc_dbtr_code) references dbtr
          on update cascade
          on delete restrict
          initially deferred;
  
alter table invc
      add constraint invc_fk_bank
          foreign key (invc_bank_code) references bank
          on update cascade
          on delete restrict
          initially deferred;
  
alter table invc
      add constraint invc_fk_prov
          foreign key (invc_prov_code) references prov
          on update cascade
          on delete restrict
          initially deferred;
  
alter table invc
      add constraint invc_fk_patn
          foreign key (invc_patn__sequence) references patn
          on update cascade
          on delete restrict
          initially deferred;
  
alter table invc
      add constraint invc_fk_empl
          foreign key (invc_empl_code) references empl
          on update cascade
          on delete restrict
          initially deferred;
  
alter table invc
      add constraint invc_fk_feet
          foreign key (invc_feet_code) references feet
          on update cascade
          on delete restrict
          initially deferred;
  
alter table invc
      add constraint invc_fk_rfdr
          foreign key (invc_rfdr_code) references rfdr
          on update cascade
          on delete restrict
          initially deferred;
