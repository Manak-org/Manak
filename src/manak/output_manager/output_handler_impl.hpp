namespace manak
{

std::string OutputHandler::GetPMRep(const utils::ObjectStore& entry)
{
  std::stringstream ss;

  double comp_val = *(double*)entry.Get("compare");
  PMeasure pm = *(PMeasure*)entry.Get("pmeasure");
  bool is_test = *(bool*)entry.Get("is_test");
  bool test_res = true;

  if(is_test)
  {
    test_res = *(bool*)entry.Get("test_res");
  }

  if(test_res)
  {
    if(comp_val >= 0)
    {
      double tol = *(double*)entry.Get("tolerance");

      int res = pm.Compare(comp_val, tol);

      if(res > 0)
        ss << "+";
      else if(res < 0)
        ss << "-";
      ss << pm << "(" << comp_val << ")";
    }
    else
      ss << pm;
  }
  else ss << "X";

  return ss.str();
}

}
