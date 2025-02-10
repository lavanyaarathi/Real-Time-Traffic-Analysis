const express = require('express');
const cors = require('cors');
const { exec } = require('child_process');

const app = express();
app.use(cors());
app.use(express.json());

app.post('/calculate-path', (req, res) => {
  const { src, dest } = req.body;
  
  // Call the C++ executable with src and dest as arguments
  exec(`./traffic_analysis ${src} ${dest}`, (error, stdout, stderr) => {
    if (error) {
      console.error(`Error executing C++ code: ${error}`);
      res.status(500).send("Internal server error");
      return;
    }
    res.json({ path: stdout.trim() }); // Send output back to frontend
  });
});

const PORT = 3000;
app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});
