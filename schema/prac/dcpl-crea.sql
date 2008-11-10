--
--   Copyright 2008 X=X Computer Software Trust
--                   Kangaroo Ground Australia 3097
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



create table dcpl                       -- Document Printing Log
(
 dcpl_docs__sequence integer not null,                  -- document id
 dcpl_access_type text not null default 'Viewed',       -- access type
 --
 -- standard headers
 --
 dcpl__sequence serial not null,                        -- Row-ID
 dcpl__timestamp timestamp default now() not null,      -- Timestamp
 dcpl__user_entry text default CURRENT_USER not null,   -- Entered by
 dcpl__status char default text 'N' not null,           -- Record status
 --
 -- constraints
 --
 primary key(dcpl__sequence),
 constraint dcpl_fk_docs
            foreign key(dcpl_docs__sequence)
            references docs
            on update cascade
            on delete cascade
            initially deferred,
 constraint dcpl_check_access_type
            check (dcpl_access_type in ('Viewed', 'Printed') )
 ) with oids;

grant select,insert,update on dcpl to public;
grant select,insert,update on dcpl_dcpl__sequence_seq to public;

select create_metadata('dcpl');
update mtcl set mtcl_title = 'Document Printing Log' where mtcl_name = 'dcpl';

-- update history;
\set mttb_name '\'dcpl\''
\set mttb_rcs_header '\'$Id: my-macros.dm,v 1.37 2007/02/19 08:18:43 source Exp $\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

