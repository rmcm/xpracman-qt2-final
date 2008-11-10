--    $Id$
--
--  Copyright 2001 X=X Computer Software Trust
--                  Kangaroo Ground Australia 3097
--
--
-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License published by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
-- 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
-- 
-- Report problems and direct all questions to:
-- 
--     Rex McMaster, rmcm@compsoft.com.au
-- 
--

-- add 3 extra fields - see dbtr-crea.sql v1.6->1.7

-- add columns
alter table patn add column patn_patf_code text;

-- update new values
update patn set patn_patf_code = '-';
insert into patf(patf_code)
       (select patn_patf_code from patn except select patf_code from patf);
-- set defaults
alter table patn alter column patn_patf_code set default text '-';

-- add metadata
select create_metadata('patn');

-- add constraint
alter table patn 
  add constraint patn_fk_patf
                 foreign key(patn_patf_code)
                 references patf
                 on update cascade
                 on delete set default
                 initially deferred;
