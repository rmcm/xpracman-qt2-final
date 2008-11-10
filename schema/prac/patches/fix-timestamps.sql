-- adjust length of timestamp fields which are used solely as dates.
--

update mtat set mtat_displen = 10
 where (mtat_type ~ 'time' or mtat_type ~ 'date') and mtat_name !~ '^....__timestamp';