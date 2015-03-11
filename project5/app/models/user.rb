class User < ActiveRecord::Base
	has_many :photos
	has_many :comments
	def password
		@typedpassword
	end
	def password=(password)
		@typedpassword = password
		self.salt = Random.rand.to_s
		digest = Digest::SHA1.hexdigest(password + salt)
		self.password_digest = digest
	end
	def password_valid? (string)
		salt = self.salt
		digest = Digest::SHA1.hexdigest(string + salt)
		if digest == self.password_digest then
			return true
		else 
			return false
		end
	end
	validates_uniqueness_of :login
end