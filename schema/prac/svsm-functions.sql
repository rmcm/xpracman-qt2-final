--     $Id$
-- 
--   Copyright 2003 X=X Computer Software Trust
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

-- ------------------------------------------------------------
-- Perform a multiservice reduction on the items of an invoice.
-- ... the services must be on the same day, and the same invoice,
--     which are the arguments to this function. The reductions
--     as fixed at 100/50/25 ...
-- ------------------------------------------------------------

create or replace function svsm_load(text, text, text)
returns integer
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;

    query text;
    from_date text;
    to_date text;
    date_attribute text;
    stype text;
    ctype text;
    ptype text;

        
    BEGIN
      from_date := quote_literal($1);
      to_date := quote_literal($2);
      date_attribute := $3;
      stype := quote_literal(''A'');
      ctype := quote_literal(''B'');
      ptype := quote_literal(''C'');

      EXECUTE ''delete from svsm;'';

      -- SERVICES
      --
      query := ''
        insert into svsm(svsm_date_start,
                         svsm_date_end,
                         svsm_type,
                         svsm_prov_code,
                         svsm_prov_name,
                         svsm_count,
                         svsm_serv_code,
                         svsm_desc,
                         svsm_amount,
                         svsm_gst_amount)
          select '' || from_date || '',
                 '' || to_date || '',
                 '' || stype || '',
                 invc_prov_code,
                 prov_name,
                 count(*),
                 svpf_serv_code,
                 serv_desc,
                 sum(svpf_amount),
                 sum(svpf_gst_amount)
            from svpf,invc,serv,prov
           where date('' || date_attribute || '') between '' || from_date || ''
                                                        and '' || to_date || ''
             and svpf_invc__sequence = invc__sequence
             and svpf_serv_code = serv_code
             and invc_prov_code = prov_code
           group by invc_prov_code, prov_name, svpf_serv_code, serv_desc;'';

      -- RAISE NOTICE ''%'', query;
      EXECUTE query;

      -- this should return the number of rows inserted
      return 0;
    END;'
  LANGUAGE 'plpgsql';

-- update history;
\set mttb_name '\'svsm\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
