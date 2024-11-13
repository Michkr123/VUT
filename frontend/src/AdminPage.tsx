import { Button } from "@mui/material";
import { Link } from "react-router-dom";

const AdminPage = () => (
  <div className="container mx-auto px-4 py-8">
    <div className="bg-white rounded-lg shadow p-6">
      <p className="text-2xl font-bold mb-3">Správa akcí:</p>
      <Button variant="contained">
        <Link to="/admin/new_event">Vytvořit novou akci</Link>
      </Button>
    </div>
  </div>
);

export default AdminPage;
