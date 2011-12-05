--  -*- sql -*-
--
--    $Id$

--  Copyright 2001 X=X Computer Software Trust
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


--
-- Create a function to provide a descriptive credit summary
--

create or replace function cred_summary(int,int)
returns text as $$

    DECLARE
    tmp_invc__sequence ALIAS for $1;
    maxrows ALIAS for $2;
    tmp_rec record;
    eol text;
    count integer;
    descr text;
    other_desc text;
    other_balance numeric;
    pad text;

    BEGIN

    eol := '';
    count := 0;
    other_balance := 0;
    other_desc = '';
    descr := '';
    pad := ' ';

    -- collect the credits

    for tmp_rec in EXECUTE '
      select    cred_paym__sequence,
                (cred_amount + cred_gst_amount) as cred_total,
                short_date(cred__timestamp) as cred_date,
                cred__timestamp,
                rpad( coalesce(paym_drawer, coalesce(tdtp_desc, tdtp_code))
                        ,10,chr(32)) as drawer
      from      cred,paym,tdtp
      where     cred_invc__sequence = ' || tmp_invc__sequence || '
      and       cred_paym__sequence = paym__sequence
      and       paym_tdtp_code = tdtp_code
      order by  cred__timestamp desc;'

    LOOP
        count := count + 1;

        if ( count > maxrows ) then
           other_balance := other_balance + tmp_rec.cred_total::numeric;
        else
          descr := descr || eol ||
                  '#'|| tmp_rec.cred_paym__sequence ||
                  ' ' || tmp_rec.cred_date ||
                  ' ' || tmp_rec.drawer ||
                  ' ' || to_char(tmp_rec.cred_total,'9999999999.99');
        end if;
        eol = '<br>';
    END LOOP;

    if ( other_balance > 0 ) then
      descr := descr || eol ||
              rpad('... other payments (' || count-maxrows || ')',29,chr(32)) ||
              to_char(other_balance, '99999.99');
    end if;

    return descr;
    END;
$$
LANGUAGE 'plpgsql';

-- update history;
\set mttb_name '\'cred\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
