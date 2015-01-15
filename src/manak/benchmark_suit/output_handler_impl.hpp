namespace manak
{

std::string OutputHandler::GetPMRep(const utils::ObjectStore& entry)
{
  std::stringstream ss;

  double comp_val = *(double*)entry.Get("compare");
  PMeasure pm = *(PMeasure*)entry.Get("pmeasure");

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
  return ss.str();
}

}
